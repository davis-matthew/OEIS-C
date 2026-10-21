/*
 * A073700  a(1) = 1, a(n) = Floor[(Sum of composite numbers up to n)/(Sum of
 *          primes up to n)].
 * https://oeis.org/A073700
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( (sum of composite numbers up to n) / (sum of prime
 *            numbers up to n) )
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* --- cached data ------------------------------------------------------- */
static idx_t  max_computed = 0;          /* largest n processed so far */
static size_t arr_cap      = 0;          /* current capacity in elements   */
static uint64_t *prime_sum = NULL;      /* cumulative sum of primes       */
static uint64_t *comp_sum  = NULL;      /* cumulative sum of composites   */
static uint64_t *prime_list = NULL;     /* list of primes discovered      */
static size_t   prime_cnt   = 0;         /* number of primes in prime_list */

/* --- helper: grow dynamic arrays ------------------------------------- */
static void extend_capacity(size_t needed)
{
    if (arr_cap < needed) {
        size_t new_cap = arr_cap ? arr_cap * 2 : 16;
        while (new_cap < needed) new_cap <<= 1;

        uint64_t *tmp_p_sum = (uint64_t *)realloc(prime_sum,  new_cap * sizeof(uint64_t));
        uint64_t *tmp_c_sum = (uint64_t *)realloc(comp_sum,   new_cap * sizeof(uint64_t));
        uint64_t *tmp_plist = (uint64_t *)realloc(prime_list,  new_cap * sizeof(uint64_t));

        if (!tmp_p_sum || !tmp_c_sum || !tmp_plist) abort();
        prime_sum   = tmp_p_sum;
        comp_sum    = tmp_c_sum;
        prime_list  = tmp_plist;

        if (arr_cap == 0) {                  /* first allocation */
            prime_sum[0] = 0;
            comp_sum[0]  = 0;
        }
        arr_cap = new_cap;
    }
}

/* --- helper: integer square root (floor) ------------------------------ */
static idx_t isqrt(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        if (mid * mid <= n) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return low - 1;
}

/* --- helper: test primality using discovered primes ------------------- */
static bool is_prime(idx_t v)
{
    if (v < 2) return false;
    idx_t limit = isqrt(v);
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p > limit) break;
        if (v % p == 0) return false;
    }
    return true;
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* special case as defined by OEIS */
    if (n == 1) return 1;

    /* enlarge cached arrays if necessary */
    if (max_computed < n) {
        extend_capacity((size_t)(n + 1));          /* need indices up to n */

        for (idx_t i = max_computed + 1; i <= n; ++i) {
            bool prime = is_prime(i);

            if (prime) {
                prime_sum[i] = prime_sum[i - 1] + (uint64_t)i;
                comp_sum[i]  = comp_sum[i - 1];
                prime_list[prime_cnt++] = (uint64_t)i;
            } else {
                prime_sum[i] = prime_sum[i - 1];
                if (i >= 4)   /* composites start at 4 */
                    comp_sum[i] = comp_sum[i - 1] + (uint64_t)i;
                else
                    comp_sum[i] = comp_sum[i - 1];
            }
        }
        max_computed = n;
    }

    uint64_t ps = prime_sum[n];   /* sum of primes up to n */
    uint64_t cs = comp_sum[n];    /* sum of composites up to n */

    /* ps cannot be zero for n>=2, so integer division is safe */
    return (seq_t)(cs / ps);
}

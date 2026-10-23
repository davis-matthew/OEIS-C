/*
 * A144695  Numbers n such that sigma_1(n)/sigma_0(n) = c^2, c an integer.
 * https://oeis.org/A144695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 996, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the OEIS sequence A144695: numbers n with
   σ₁(n) / σ₀(n) = c² for integer c.
   σ₁(n) – sum of divisors of n
   σ₀(n) – number of divisors of n                      */

#include <stdint.h>
#include <stdlib.h>

/* --------------------------------------------------------------------- */
/* Helper: integer square‑root for 64‑bit values (floor sqrt).            */
static uint64_t
isqrt_uint64(uint64_t x)
{
    uint64_t lo = 0, hi = 1ULL << 32;
    while (lo < hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        if (mid > 0 && mid > x / mid) hi = mid;
        else lo = mid + 1;
    }
    return lo - 1;
}

/* --------------------------------------------------------------------- */
/* Cached list of sequence terms.                                        */
static uint64_t *terms   = NULL;   /* array of found numbers          */
static size_t   terms_len = 0;      /* how many terms are stored      */
static size_t   terms_cap = 0;      /* capacity of the array          */

/* --------------------------------------------------------------------- */
/* Grow the cached array when more space is needed.                       */
static void
grow_terms(size_t needed)
{
    if (terms_cap >= needed)
        return;
    size_t new_cap = terms_cap ? terms_cap << 1 : 64;
    while (new_cap < needed)
        new_cap <<= 1;
    uint64_t *new_arr = (uint64_t *)realloc(terms, new_cap * sizeof(uint64_t));
    if (!new_arr)
        abort();                      /* memory exhaustion */
    terms = new_arr;
    terms_cap = new_cap;
}

/* --------------------------------------------------------------------- */
/* Return the n‑th term of A144695 (1‑based index).                       */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;               /* undefined for index 0 */

    /* Ensure we have at least n terms cached */
    static uint64_t next_candidate = 1;    /* smallest integer to test */

    while (terms_len < n) {
        uint64_t m = next_candidate++;

        /* Factorisation of m, computation of σ₁(m) and σ₀(m) */
        uint64_t temp = m;
        uint64_t sigma = 1;
        uint64_t d_cnt = 1;
        for (uint64_t p = 2; p * p <= temp; ++p) {
            if (temp % p != 0)
                continue;
            uint64_t exp = 0;
            uint64_t p_pow = 1;          /* p^exp initial 1 */
            while (temp % p == 0) {
                temp /= p;
                ++exp;
                p_pow *= p;
            }
            /* σ(p^exp) = (p^(exp+1)-1)/(p-1) */
            uint64_t numerator   = p_pow * p - 1;
            uint64_t denominator = p - 1;
            sigma *= numerator / denominator;
            d_cnt  *= exp + 1;
        }
        if (temp > 1) {                  /* remaining prime factor */
            uint64_t p = temp;
            /* exp = 1 */
            uint64_t numerator   = p * p - 1;  /* p^2 - 1 */
            uint64_t denominator = p - 1;      /* p - 1 */
            sigma *= numerator / denominator; /* equals p+1 */
            d_cnt *= 2;
        }

        /* Test if σ₁(m)/σ₀(m) is a perfect square integer */
        if (sigma % d_cnt == 0) {
            uint64_t ratio = sigma / d_cnt;
            uint64_t root  = isqrt_uint64(ratio);
            if (root * root == ratio) {
                grow_terms(terms_len + 1);
                terms[terms_len++] = m;
            }
        }
    }

    return (seq_t)terms[n - 1];
}

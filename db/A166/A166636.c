/*
 * A166636  Totally multiplicative sequence with a(p) = 6*(p-1) for prime p.
 * https://oeis.org/A166636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e‖n} (6·(p‑1))^e (multiplicative with a(p^e) =
 *            (6(p‑1))^e)
 * Range:     u8 n <= 11, u16 n <= 95, u32 n <= 6143, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/* ---------- Utilities ---------- */

/* integer square root: returns ⌊√n⌋ */
static idx_t isqrt_uint64(idx_t n)
{
    idx_t low = 0, high = (idx_t)0xFFFFFFFFULL + 1;      /* sqrt(2^64‑1) < 2^32 */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        idx_t sq  = mid * mid;
        if (sq <= n)
            low = mid + 1;
        else
            high = mid;
    }
    return low - 1;
}

/* prime table maintained in static storage */
static idx_t *prime_list   = NULL;
static size_t prime_count  = 0;
static idx_t  prime_limit  = 0;

/* enlarge prime table to cover all primes ≤ limit */
static void ensure_primes(idx_t limit)
{
    if (limit <= prime_limit) return;

    /* free old list */
    free(prime_list);
    prime_list = NULL;
    prime_count = 0;

    /* simple sieve upto limit */
    size_t sz = (size_t)limit + 1;
    char *is_comp = (char*)calloc(sz, 1);   /* 0 = prime candidate */

    /* allocate space for primes (start with 2000, grow as needed) */
    prime_limit = 2000;
    prime_list  = (idx_t*)malloc(prime_limit * sizeof(idx_t));

    for (idx_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            if (prime_count == prime_limit) {
                prime_limit *= 2;
                prime_list = (idx_t*)realloc(prime_list, prime_limit * sizeof(idx_t));
            }
            prime_list[prime_count++] = i;

            if (i <= limit / i) {
                for (idx_t j = i * i; j <= limit; j += i)
                    is_comp[j] = 1;
            }
        }
    }
    free(is_comp);
    prime_limit = limit;
}

/* ---------- Sequence function ---------- */

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;   /* a(1) = 1 */

    idx_t tmp   = n;
    seq_t res   = 1;
    idx_t limit = isqrt_uint64(tmp) + 1;

    /* make sure we have all primes needed for factorisation */
    ensure_primes(limit);

    /* trial division using the pre‑computed primes */
    for (size_t i = 0; i < prime_count && (idx_t)prime_list[i] * prime_list[i] <= tmp; ++i) {
        idx_t p = prime_list[i];
        if (tmp % p == 0) {
            size_t e = 0;
            do {
                tmp /= p;
                ++e;
            } while (tmp % p == 0);

            seq_t base = (seq_t)6 * ((seq_t)p - (seq_t)1);
            seq_t pow_val = 1;
            for (size_t j = 0; j < e; ++j) pow_val *= base;

            res *= pow_val;
        }
    }

    /* any remaining prime factor > sqrt(original n) has exponent 1 */
    if (tmp > 1) {
        seq_t base = (seq_t)6 * ((seq_t)tmp - (seq_t)1);
        res *= base;
    }

    return res;
}

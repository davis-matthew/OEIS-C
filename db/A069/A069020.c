/*
 * A069020  a(1) = 1; a(n) = smallest number of the form k*a(n-1) +1 divisible
 *          by n^2.
 * https://oeis.org/A069020
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = smallest number of the form k*a(n-1)+1 divisible by n^2
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;          /* array of computed a(n), index 1.. */
static idx_t  last_computed = 0;     /* largest index that has been computed */

/* Compute modular inverse of a mod mod (both < 2^64) using Extended Euclid.
   Returns 0 if inverse does not exist. */
static uint64_t modinv_uint64(uint64_t a, uint64_t mod)
{
    int64_t t = 0, newt = 1;
    int64_t r = (int64_t)mod, newr = (int64_t)a % (int64_t)mod;
    while (newr != 0) {
        int64_t q = r / newr;
        int64_t tmp = newt;
        newt = t - q * newt;
        t = tmp;
        tmp = newr;
        newr = r - q * newr;
        r = tmp;
    }
    if (r != 1) return 0;                /* no inverse */
    if (t < 0) t += (int64_t)mod;        /* ensure positive */
    return (uint64_t)t;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* not used, but safe */

    /* initialise cache if needed */
    if (!cache) {
        cache = (seq_t *)calloc(2, sizeof(seq_t));  /* indices 0,1 */
        cache[1] = 1;                          /* a(1) = 1 */
        last_computed = 1;
    }

    if (n <= last_computed) return cache[n];

    /* enlarge cache to reach index n */
    size_t oldsize = (size_t)(last_computed + 1);
    size_t newsize = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
    if (!tmp) abort();                      /* memory allocation failed */
    cache = tmp;
    /* initialise new entries to 0 */
    for (idx_t i = last_computed + 1; i <= n; ++i)
        cache[i] = 0;

    /* compute terms from last_computed+1 up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        /* if previous term is 0, the sequence stops here */
        if (prev == 0) {
            cache[i] = 0;
            last_computed = i;
            break;
        }

        seq_t mod = (seq_t)i * (seq_t)i;            /* n^2 */
        seq_t prev_mod = prev % mod;                /* a(n-1) mod n^2 */

        /* All numbers here are < 2^64 because i^2 <= 10^8 */
        uint64_t a_mod_u = (uint64_t)prev_mod;
        uint64_t mod_u   = (uint64_t)i * (uint64_t)i;

        uint64_t inv = modinv_uint64(a_mod_u, mod_u);
        if (inv == 0) {                            /* no solution, stop */
            cache[i] = 0;
            last_computed = i;
            break;
        }

        seq_t k = ((mod - (seq_t)1) * (seq_t)inv) % mod;
        seq_t val = k * prev + (seq_t)1;            /* a(i) = k*a(i-1)+1 */

        cache[i] = val;
    }

    last_computed = (last_computed > n) ? last_computed : n;
    return cache[n];
}

/*
 * A050947  Values of Soviet money, 1961-1991 (in kopecks; 100 kopecks = 1
 *          rouble).
 * https://oeis.org/A050947
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = irregular[0..14] for n < 15.
   For n ≥ 15 let offset = n-15,
   e = 4 + offset/3.
   If offset mod 3 == 0  →  a(n) = 2 * 10^e
   If offset mod 3 == 1  →  a(n) = 5 * 10^e
   If offset mod 3 == 2  →  a(n) = 10^(e+1)  */

/* Required includes for memory allocation and size types. */
#include <stdlib.h>
#include <stddef.h>

/* Irregular starting terms of the sequence. */
static const seq_t irregular[15] =
{
    (seq_t)1, (seq_t)2, (seq_t)3, (seq_t)5,
    (seq_t)10, (seq_t)15, (seq_t)20, (seq_t)50,
    (seq_t)100, (seq_t)300, (seq_t)500, (seq_t)1000,
    (seq_t)2500, (seq_t)5000, (seq_t)10000
};

/* Cache for powers of ten, computed lazily. */
static seq_t *pow_cache = NULL;      /* array of 10^k for k >= 0 */
static size_t pow_cache_len = 0;     /* number of cached values (pow_cache_len == k+1 if 10^k cached) */

/* Ensure that 10^max_exponent is cached. */
static void ensure_pow_cache(idx_t max_exponent)
{
    if (pow_cache_len == 0) {
        pow_cache = (seq_t*)malloc(sizeof(seq_t));
        if (!pow_cache) return;           /* allocation failure – will be handled by caller */
        pow_cache[0] = (seq_t)1;
        pow_cache_len = 1;
    }
    if (pow_cache_len <= (size_t)max_exponent) {
        size_t new_len = (size_t)max_exponent + 1;
        seq_t *tmp = (seq_t*)realloc(pow_cache, new_len * sizeof(seq_t));
        if (!tmp) return;                  /* allocation failure – will be handled by caller */
        pow_cache = tmp;
        for (size_t i = pow_cache_len; i < new_len; ++i) {
            pow_cache[i] = pow_cache[i-1] * (seq_t)10;
        }
        pow_cache_len = new_len;
    }
}

/* The sequence function. */
seq_t a(idx_t n)
{
    if (n < (idx_t)15) {
        return irregular[(size_t)n];
    }

    /* compute offset beyond the irregular block */
    idx_t offset = n - (idx_t)15;
    idx_t e = (idx_t)4 + offset / (idx_t)3;
    idx_t r = offset % (idx_t)3;

    /* we need 10^e and possibly 10^(e+1) */
    ensure_pow_cache(e + (r == 2 ? (idx_t)1 : (idx_t)0));

    if (r == 0) {
        return (seq_t)2 * pow_cache[(size_t)e];
    } else if (r == 1) {
        return (seq_t)5 * pow_cache[(size_t)e];
    } else { /* r == 2 */
        return pow_cache[(size_t)(e + 1)];
    }
}

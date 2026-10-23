/*
 * A134863  Wythoff BAB numbers.
 * https://oeis.org/A134863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*A(n) + 3*B(n) - 1 = 5*A(n) + 3*n - 1 where A(n) are the
 *            lower Wythoff numbers (A000201), B(n) = A(n) + n are the upper
 *            Wythoff numbers (A001950)
 * Range:     u8 n <= 23, u16 n <= 5909, u32 n <= 9827, u64 n <= 9827,
 *            u128 n <= 9827, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* cached lower Wythoff numbers A(k)   (1‑based: A_cache[0] = A(1)) */
static seq_t *A_cache      = NULL;      /* array of computed A(n) */
static size_t A_cache_len  = 0;         /* number of entries in A_cache */

/* bitset of used integers (positive integers that have appeared as A or B) */
static unsigned char *used  = NULL;      /* 0‑based index, max value = used_len-1 */
static size_t used_len      = 0;         /* current allocated size of 'used' */

/* next candidate integer that might become the next A_k (positive integer) */
static size_t candidate   = 1;

/* ensure 'used' array can index up to 'upto' (inclusive) */
static void ensure_used(size_t upto)
{
    if (upto < used_len)
        return;                           /* already large enough */

    size_t new_len = used_len ? used_len << 1 : 64;   /* start from 64 */
    while (new_len <= upto)
        new_len <<= 1;                     /* grow until > upto */

    unsigned char *tmp = (unsigned char *)realloc(used, new_len);
    if (!tmp) abort();                     /* allocation failure */
    /* zero initialise the newly allocated tail */
    memset(tmp + used_len, 0, new_len - used_len);
    used = tmp;
    used_len = new_len;
}

/* ensure the first 'n' lower Wythoff numbers are computed and cached */
static void ensure_A_cache(idx_t n)
{
    if ((size_t)n <= A_cache_len)
        return;                               /* already computed */

    /* enlarge cache array if necessary */
    if (A_cache_len < (size_t)n) {
        seq_t *tmp = (seq_t *)realloc(A_cache, (size_t)n * sizeof(seq_t));
        if (!tmp) abort();                    /* allocation failure */
        A_cache = tmp;
    }

    /* generate A(k) for k = A_cache_len + 1 .. n */
    for (size_t k = A_cache_len + 1; k <= (size_t)n; ++k) {
        /* find smallest unused positive integer = next A_k */
        while (candidate < used_len && used[candidate])
            ++candidate;
        /* ensure array can index B = candidate + k */
        size_t B = candidate + k;
        ensure_used(B);

        /* mark A_k and its companion B as used */
        used[candidate] = 1;
        used[B]         = 1;

        /* store the computed A(k) */
        A_cache[k - 1] = (seq_t)candidate;

        /* advance to next candidate for the subsequent iteration */
        ++candidate;
    }

    A_cache_len = (size_t)n;
}

/* public sequence function */
seq_t a(idx_t n)
{
    /* sequence is defined for n >= 1 */
    ensure_A_cache(n);
    seq_t A = A_cache[(size_t)n - 1];
    return (seq_t)5 * A + (seq_t)3 * (seq_t)n - (seq_t)1;
}

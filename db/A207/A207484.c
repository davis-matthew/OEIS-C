/*
 * A207484  Number of nX4 0..1 arrays avoiding 0 0 0 and 0 1 1 horizontally
 *          and 0 1 1 and 1 1 0 vertically.
 * https://oeis.org/A207484
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n‑1) – 5*a(n‑2) – 106*a(n‑3) + 209*a(n‑4) +
 *            518*a(n‑5) * – 1487*a(n‑6) – 1150*a(n‑7) + 4949*a(n‑8) +
 *            938*a(n‑9) * – 8999*a(n‑10) + 706*a(n‑11) + 9184*a(n‑12) –
 *            2042*a(n‑13) * – 5056*a(n‑14) + 1552*a(n‑15) + 1334*a(n‑16) –
 *            472*a(n‑17) * – 128*a(n‑18) + 48*a(n‑19) for n > 20 * Initial
 *            va...
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 37, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Constants */
static const seq_t INIT[20] = {
    9ULL, 81ULL, 217ULL, 530ULL, 1459ULL,
    3947ULL, 11306ULL, 32445ULL, 96201ULL, 286920ULL,
    873201ULL, 2673027ULL, 8288284ULL, 25824249ULL, 81128201ULL,
    255825008ULL, 811028901ULL, 2578371055ULL, 8225731808ULL,
    26296176189ULL
};

static const int coeffs[19] = {
    8,   -5,  -106,  209,  518,
   -1487, -1150, 4949, 938, -8999,
    706, 9184, -2042, -5056, 1552,
    1334, -472, -128,  48
};

/* Cached values (0‑based index: a(1) at cache[0]) */
static seq_t *cache = NULL;
static size_t cached = 0;  /* number of terms stored */

/* Ensure the cache holds at least n terms (1‑based index). */
static void ensure_cache(idx_t n)
{
    if (cached >= (size_t)n)
        return;          /* already enough */

    size_t newsize = (size_t)n;

    /* allocate or grow storage */
    #ifdef __cplusplus
    seq_t *tmp = new seq_t[newsize];
    #else
    seq_t *tmp = (seq_t*)malloc(newsize * sizeof(seq_t));
    #endif
    if (!tmp)
        abort();   /* allocation failure */

    /* copy any existing values */
    for (size_t i = 0; i < cached; ++i)
        tmp[i] = cache[i];

    /* initialize first 20 terms (if needed) */
    size_t init_len = 20;
    if (init_len > newsize)
        init_len = newsize;
    for (size_t i = cached; i < init_len; ++i)
        tmp[i] = INIT[i];

    /* compute further terms using the recurrence (for n > 20) */
    size_t start = (cached < 20) ? 20 : cached;
    for (size_t i = start; i < newsize; ++i) {
        seq_t pos = 0, neg = 0;
        for (int k = 1; k <= 19; ++k) {
            const seq_t term = tmp[i - k];
            const int coeff = coeffs[k - 1];
            if (coeff >= 0)
                pos += term * (seq_t)coeff;
            else
                neg += term * (seq_t)(-coeff);
        }
        tmp[i] = pos - neg;
    }

    /* free old array */
    #ifdef __cplusplus
    delete[] cache;
    #else
    free(cache);
    #endif

    cache  = tmp;
    cached = newsize;
}

seq_t a(idx_t n)
{
    if (n == 0)         /* offset is 1; undefined for n < 1 */
        return (seq_t)0;
    ensure_cache(n);    /* guarantee term exists */
    return cache[n - 1]; /* cache is 0‑based */
}

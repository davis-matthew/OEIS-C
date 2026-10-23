/*
 * A041855  Denominators of continued fraction convergents to sqrt(449).
 * https://oeis.org/A041855
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 378942664*a(n-17) + a(n-34) for n > 33 Initial values
 *            (generated from the continued‑fraction of √449): a(0)=1, a(1)=5,
 *            a(2)=16, a(3)=21, a(4)=37, a(5)=58, a(6)=443, a(7)=501,
 *            a(8)=2948, a(9)=15241, a(10)=18189, a(11)=142564, a(12)=160753,
 *            a(13)=303317, a(14)=464070, a(15)=1695527, a(16)=89...
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 18, u64 n <= 39, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *initial_q = NULL;          /* first 34 denominators */
static const idx_t MAX_INIT = 34;

/* Generate the first 34 denominators of the continued fraction of √449. */
static void compute_initial_q(void)
{
    if (initial_q != NULL)
        return;            /* already computed */

    initial_q = (seq_t*)malloc(MAX_INIT * sizeof(seq_t));
    if (!initial_q)
        return;            /* malloc failure – results will be 0 */

    /* Parameters for continuing fraction of sqrt(449) */
    const uint64_t D      = 449ULL;   /* the square‑free part */
    const uint64_t a0     = 21ULL;    /* floor(sqrt(449)) */
    uint64_t m = 0;
    uint64_t d = 1;
    uint64_t a = a0;

    /* Initial denominators for recurrence q_{-2}=1, q_{-1}=0  (see CF theory) */
    uint64_t q_nm2 = 1;   /* q_{-2} */
    uint64_t q_nm1 = 0;   /* q_{-1} */

    for (idx_t i = 0; i < MAX_INIT; ++i) {
        if (i == 0) {
            a = a0;
        } else {
            /* Next complete extraction step */
            m = d * a - m;
            d = (D - m * m) / d;
            a = (a0 + m) / d;
        }
        uint64_t q = a * q_nm1 + q_nm2;
        initial_q[i] = (seq_t)q;
        q_nm2 = q_nm1;
        q_nm1 = q;
    }
}

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;   /* memoised values up to current size */
    static size_t cache_size = 0;   /* current allocated size */

    /* Ensure initial terms are computed */
    compute_initial_q();

    /* Expand cache to cover index n, if necessary */
    if (cache_size <= (size_t)n) {
        size_t new_size = (size_t)n + 1;
        seq_t *new_cache = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache)
            return 0;            /* malloc failure – return 0 as a fallback */

        /* Compute new entries sequentially */
        for (size_t i = cache_size; i < new_size; ++i) {
            if (i < MAX_INIT) {
                /* Use the explicitly generated first 34 terms */
                new_cache[i] = initial_q[i];
            } else {
                /* Recurrence: a(n) = 378942664*a(n‑17) + a(n‑34) */
                new_cache[i] = (seq_t)378942664 * new_cache[i-17] + new_cache[i-34];
            }
        }

        cache = new_cache;
        cache_size = new_size;
    }

    return cache[(size_t)n];
}

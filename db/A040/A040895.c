/*
 * A040895  Continued fraction for sqrt(926).
 * https://oeis.org/A040895
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) = nth partial quotient of the simple continued fraction of
 *            sqrt(926)
 * Range:     u8 n <= 1, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(0) = floor(sqrt(926)) = 30, a(1)=2, a(2)=3, ...  as given in OEIS A040895 */

#include <stdlib.h>

/* Standard parameters of the continued fraction for sqrt(926) */
#define D_CONST 926ULL          /* N non‑zero integer D */
#define A0_CONST 30U            /* floor(sqrt(926)) */

/* The function must be externally linked */
seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;      /* dynamic array of computed terms   */
    static size_t cache_len = 0;     /* number of terms currently stored */
    static size_t cache_cap = 0;     /* allocated capacity (in terms) */

    /* State variables representing the last computed step of the CF algorithm */
    static seq_t m = 0;               /* m_k   (for the last computed k)   */
    static seq_t d = 1;               /* d_k   */
    static seq_t a_k = 0;             /* a_k   (current a_k)               */

    /* Initialise the cache and the algorithm state on first call */
    if (cache == NULL) {
        cache_cap = 16;                     /* arbitrary initial capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL)                 /* allocation failure: return 0 */
            return (seq_t)0;

        cache_len = 1;
        cache[0] = (seq_t)A0_CONST;         /* a(0) */
        m = 0;                              /* initial m_0 */
        d = 1;                              /* initial d_0 */
        a_k = (seq_t)A0_CONST;              /* a_0   */
    }

    /* If the required index is already cached, just return it */
    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    /* Extend the cache up to index n, generating new terms on demand */
    while (cache_len <= (size_t)n) {
        /* Compute next terms of the continued fraction algorithm */
        seq_t m_next = d * a_k - m;
        seq_t d_next = ((seq_t)D_CONST - m_next * m_next) / d;
        seq_t a_next = ((seq_t)A0_CONST + m_next) / d_next;

        /* Grow the cache if necessary */
        if (cache_len >= cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (new_cache == NULL)          /* allocation failure: return 0 */
                return (seq_t)0;
            cache = new_cache;
            cache_cap = new_cap;
        }

        /* Store the newly computed term */
        cache[cache_len] = a_next;
        cache_len++;

        /* Update state to the new index for the next iteration */
        m = m_next;
        d = d_next;
        a_k = a_next;
    }

    return cache[(size_t)n];
}

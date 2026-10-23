/*
 * A041765  Denominators of continued fraction convergents to sqrt(403).
 * https://oeis.org/A041765
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 1339756*a(n-10) - a(n-20) for n>=20, initial values:
 *            a(0)=1, a(1)=13, a(2)=27, a(3)=40, a(4)=147, a(5)=187, a(6)=708,
 *            a(7)=895, a(8)=2498, a(9)=33369, a(10)=1337258, a(11)=17417723,
 *            a(12)=36172704, a(13)=53590427, a(14)=196943985,
 *            a(15)=250534412, a(16)=948547221, a(17)=1199081633, a(18)=3...
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 30, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* cache for previously computed terms */
static seq_t *cache = NULL;      /* array of seq_t values */
static idx_t cache_size  = 0;    /* number of terms stored */
static idx_t cache_capacity = 0; /* current allocated size */

/* compute the first 20 terms from the continued fraction of sqrt(403) */
static void init_initial_terms(void)
{
    const idx_t required = 20;
    /* allocate/reallocate to hold the 20 initial values */
    seq_t *arr = (seq_t*)realloc(cache, required * sizeof(seq_t));
    if (!arr) return;                /* keep cache= NULL on failure */
    cache = arr;
    cache_capacity = required;

    /* parameters for the continued fraction of sqrt(403) */
    const idx_t a0 = 20;             /* floor(sqrt(403)) */
    idx_t m   = 0;
    idx_t d   = 1;
    idx_t a_i = a0;                  /* current partial quotient */

    /* convergents: p_{-2}=0, p_{-1}=1; q_{-2}=1, q_{-1}=0 */
    seq_t p_nm2 = 0, p_nm1 = 1;
    seq_t q_nm2 = 1, q_nm1 = 0;

    for (idx_t i = 0; i < required; ++i)
    {
        /* new denominator q_n = a_i*q_{n-1} + q_{n-2} */
        seq_t q_n = (seq_t)a_i * q_nm1 + q_nm2;
        cache[i] = q_n;

        /* update numerator and denominator for next step */
        seq_t p_n = (seq_t)a_i * p_nm1 + p_nm2;
        p_nm2 = p_nm1; p_nm1 = p_n;
        q_nm2 = q_nm1; q_nm1 = q_n;

        /* compute next partial quotient */
        idx_t m_next = d * a_i - m;
        idx_t d_next = (403 - m_next * m_next) / d;
        a_i = (a0 + m_next) / d_next;

        m = m_next;
        d = d_next;
    }

    cache_size = required;
}

/* compute the n-th term of A041765 */
seq_t a(idx_t n)
{
    /* ensure initial terms are available */
    if (cache == NULL)
        init_initial_terms();

    /* values with index < 20 are already in the cache */
    if (n < cache_size)
        return cache[n];

    /* extend the cache up to index n */
    for (idx_t i = cache_size; i <= n; ++i)
    {
        /* recurrence: a(i) = 1339756*a(i-10) - a(i-20) */
        seq_t val = (seq_t)1339756 * cache[i - 10] - cache[i - 20];

        /* grow capacity if necessary */
        if (cache_size >= cache_capacity)
        {
            idx_t new_cap = cache_capacity ? (cache_capacity * 2) : 32;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;     /* allocation failure: return 0 */
            cache = tmp;
            cache_capacity = new_cap;
        }

        cache[cache_size++] = val;
    }

    return cache[n];
}

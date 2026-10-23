/*
 * A042289  Denominators of continued fraction convergents to sqrt(670).
 * https://oeis.org/A042289
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 11582422*a(n-14) - a(n-28) for n>27. The first 28 terms
 *            (a(0)..a(27)) are generated from the continued‑fraction
 *            convergents of sqrt(670)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 39, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h> /* malloc, realloc */

static seq_t *cache = NULL;   /* cache of computed terms */
static idx_t cache_size = 0;  /* number of terms stored */

/* Initialise the cache with the first 28 terms from the continued
   fraction for sqrt(670) (denominators of the convergents). */
static void init_cache(void)
{
    const idx_t init_n = 28;                 /* need terms a(0)…a(27) */
    const idx_t D      = 670;                /* radicand */
    const idx_t a0     = 25;                 /* floor(sqrt(670)) */

    /* Allocate space for the 28 terms.  Cast required for C++. */
    cache = (seq_t *)malloc(init_n * sizeof(seq_t));
    if (!cache) return;                     /* allocation failed */

    /* Continued‑fraction state variables. */
    idx_t m      = 0;
    idx_t d      = 1;
    idx_t a      = a0;                      /* current partial quotient */

    seq_t Qprev2 = 1;                       /* Q_{-2} */
    seq_t Qprev1 = 0;                       /* Q_{-1} */

    for (idx_t k = 0; k < init_n; ++k) {
        /* Q_k = a_k * Q_{k-1} + Q_{k-2} */
        seq_t Q = (seq_t)a * Qprev1 + Qprev2;
        cache[k] = Q;

        /* Generate next partial quotient. */
        idx_t m_next = d * a - m;
        idx_t d_next = (D - m_next * m_next) / d;
        idx_t a_next = (a0 + m_next) / d_next; /* floor((a0 + m_next)/d_next) */

        m = m_next;
        d = d_next;
        a = a_next;

        Qprev2 = Qprev1;
        Qprev1 = Q;
    }

    cache_size = init_n;
}

/* Ensure that the cache contains terms up to index n.
   Recurrence is applied for n ≥ 28. */
static void ensure_cache(idx_t n)
{
    if (!cache) init_cache();
    if (n < cache_size) return;

    /* Resize cache to accommodate up to n. */
    idx_t new_size = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) return;       /* allocation failed – keep old cache */

    cache = tmp;
    for (idx_t i = cache_size; i <= n; ++i) {
        cache[i] = (seq_t)11582422 * cache[i - 14] - cache[i - 28];
    }
    cache_size = new_size;
}

/* Public function: returns a(n).  Works for any order of calls. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return (n < cache_size) ? cache[n] : (seq_t)0;
}

/*
 * A042918  Numerators of continued fraction convergents to sqrt(991).
 * https://oeis.org/A042918
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 18, u64 n <= 34, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of continued fraction convergents to sqrt(991) */
#include <stdlib.h>

static seq_t *cache = NULL;      /* Cache of computed numerators */
static size_t  cap   = 0;        /* Allocated capacity of the cache */
static size_t  len   = 0;        /* Number of terms currently cached */

static idx_t last_index = (idx_t)-1; /* Highest n for which cache[n] is valid */

static uint64_t m_cur = 0;      /* Current m value in the continued fraction */
static uint64_t d_cur = 1;      /* Current d value in the continued fraction */
static uint64_t a_cur = 31;     /* Current a value in the continued fraction (a0 first) */
static const uint64_t D  = 991; /* D for sqrt(991) */
static const uint64_t A0 = 31;  /* a0 = floor(sqrt(991)) */

static seq_t prev1 = 1; /* p_{-1} */
static seq_t prev2 = 0; /* p_{-2} */

/* Increase the cache size to hold at least 'need' elements. */
static void grow_cache(size_t need)
{
    if (need <= cap)
        return;
    size_t newcap = cap ? cap * 2 : 16;
    while (newcap < need)
        newcap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    if (!tmp) return; /* allocation failure silently ignored */
    cache = tmp;
    cap = newcap;
}

/* Compute the next partial quotient and its numerator, storing it in the cache. */
static void compute_next_term(void)
{
    /* p_n = a_n * p_{n-1} + p_{n-2} */
    seq_t curr = a_cur * prev1 + prev2;

    /* Store the new numerator */
    grow_cache(len + 1);
    cache[len++] = curr;

    /* Prepare values for the next iteration */
    prev2 = prev1;
    prev1 = curr;

    /* Continue the continued‑fraction expansion */
    uint64_t m_next = d_cur * a_cur - m_cur;
    uint64_t d_next = (D - m_next * m_next) / d_cur;
    uint64_t a_next = (A0 + m_next) / d_next;

    m_cur = m_next;
    d_cur = d_next;
    a_cur = a_next;
}

/* Return the nth numerator of the convergents to sqrt(991). */
seq_t a(idx_t n)
{
    /* Initialise on the first call */
    if (len == 0) {
        grow_cache(16);
        /* Compute the first term (n = 0) */
        compute_next_term(); /* stores term 0 */
        last_index = 0;
    }

    /* Extend the cache up to the requested index */
    while (last_index < n) {
        compute_next_term();
        last_index++;
    }

    return cache[n];
}

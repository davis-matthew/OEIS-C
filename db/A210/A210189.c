/*
 * A210189  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A210190; see the Formula section.
 * https://oeis.org/A210189
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the nth coefficient (row‐wise, constant term first) of
 *            the triangle of coefficients of the polynomials u(n,x) defined
 *            by: u(1,x) = 1, v(1,x) = 1; u(n,x) = u(n-1,x) + v(n-1,x) + 1,
 *            v(n,x) = 2x*u(n-1,x) + v(n-1,x) + 1
 * Range:     u8 n <= 18, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* flattened coefficients of u(n,x) */
static size_t cache_len = 0;     /* number of terms stored in cache          */
static size_t cache_cap = 0;     /* capacity of the cache                    */

static seq_t *u = NULL;          /* coefficients of current u(n,x)          */
static seq_t *v = NULL;          /* coefficients of current v(n,x)          */
static idx_t u_deg = 0;          /* degree of current u (>=0)               */
static idx_t v_deg = 0;          /* degree of current v (>=0)               */
static idx_t cur_n = 0;          /* last n that has been fully processed   */

/* Append a new term to cache, growing it if necessary. */
static inline void cache_push(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        if (new_cap < cache_len + 1) new_cap = cache_len + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* One-time initialization of the data structures. */
static void init(void)
{
    /* u(1) = v(1) = 1 */
    u_deg = v_deg = 0;
    u = (seq_t*)calloc(u_deg + 1, sizeof(seq_t));
    v = (seq_t*)calloc(v_deg + 1, sizeof(seq_t));
    u[0] = (seq_t)1;
    v[0] = (seq_t)1;

    cur_n = 1;                 /* n=1 has been computed */
    cache_push(u[0]);          /* a(1) = 1 */
}

/* Return the nth term (1‑based) of the sequence. */
seq_t a(idx_t n)
{
    if (!cache) init();                   /* first call */

    if (cache_len >= (size_t)n)           /* already computed? */
        return cache[n - 1];

    /* Compute successive rows until we reach the desired term. */
    while (cache_len < (size_t)n) {
        idx_t next_n = cur_n + 1;

        idx_t new_u_deg = (next_n - 1) / 2; /* floor((next_n-1)/2) */
        idx_t new_v_deg =  next_n / 2;      /* floor(next_n/2)     */

        seq_t *u_next = (seq_t*)calloc(new_u_deg + 1, sizeof(seq_t));
        seq_t *v_next = (seq_t*)calloc(new_v_deg + 1, sizeof(seq_t));

        /* Constant terms (degree 0). */
        u_next[0] = u[0] + v[0] + (seq_t)1;
        v_next[0] = v[0] + (seq_t)1;

        /* Higher‑degree coefficients of u_next. */
        for (idx_t i = 1; i <= new_u_deg; ++i) {
            seq_t ui = (i <= u_deg) ? u[i] : (seq_t)0;
            seq_t vi = (i <= v_deg) ? v[i] : (seq_t)0;
            u_next[i] = ui + vi;
        }

        /* Coefficients of v_next (degree ≥ 1). */
        for (idx_t i = 1; i <= new_v_deg; ++i) {
            seq_t vi = (i <= v_deg) ? v[i] : (seq_t)0;
            seq_t u_prev = (i-1 <= u_deg) ? u[i-1] : (seq_t)0;
            v_next[i] = vi + u_prev + u_prev; /* + 2*u_prev */
        }

        /* Store the new row of u coefficients into the cache. */
        for (idx_t i = 0; i <= new_u_deg; ++i)
            cache_push(u_next[i]);

        /* Advance to the next row. */
        free(u);
        free(v);
        u = u_next;
        v = v_next;
        u_deg = new_u_deg;
        v_deg = new_v_deg;
        cur_n = next_n;
    }

    return cache[n - 1];
}

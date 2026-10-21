/*
 * A069009  Let M denote the 6 X 6 matrix with rows / 1,1,1,1,1,1 /
 *          1,1,1,1,1,0 / 1,1,1,1,0,0 / 1,1,1,0,0,0 / 1,1,0,0,0,0 /
 *          1,0,0,0,0,0 / and A(n) the vector (x(n),y(n),z(n),t(n),u(n),v(n))
 *          = M^n*A where A is the vector (1,1,1,1,1,1); then a(n) = t(n).
 * https://oeis.org/A069009
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = t(n) where t(n) is obtained by iterating the linear
 *            recurrence derived from M^n * (1,1,1,1,1,1)^T. The recurrence
 *            for the state vector (x,y,z,t,u,v) is x' = x + y + z + t + u + v
 *            y' = x + y + z + t + u z' = x + y + z + t t' = x + y + z u' = x
 *            + y v' = x with initial values x=y=z=t=u=v = 1....
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Compute the n‑th term of OEIS A069009.  */
seq_t a(idx_t n) {
    /* static memoisation data */
    static bool initialized = false;
    static seq_t *t_cache = NULL;          /* array of t(n) values */
    static size_t cache_cap = 0;           /* allocated capacity */
    static size_t cache_len = 0;           /* number of stored terms */
    static seq_t x = 1, y = 1, z = 1, t = 1, u = 1, v = 1; /* current state */

    /* initialise on first call */
    if (!initialized) {
        cache_cap = 16;                    /* start with some capacity */
        t_cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!t_cache) {
            /* in a real program we would handle allocation failure */
            while (true);                 /* halt execution */
        }
        t_cache[0] = 1;                    /* a(0) */
        cache_len = 1;
        initialized = true;
    }

    /* if requested n already computed */
    if ((size_t)n < cache_len) {
        return t_cache[(size_t)n];
    }

    /* extend memoisation up to n */
    while ((size_t)n >= cache_len) {
        /* compute next state */
        seq_t nx = x + y + z + t + u + v;
        seq_t ny = x + y + z + t + u;
        seq_t nz = x + y + z + t;
        seq_t nt = x + y + z;
        seq_t nu = x + y;
        seq_t nv = x;

        /* append the new t value */
        if (cache_len >= cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(t_cache, new_cap * sizeof(seq_t));
            if (!tmp) {
                while (true);                 /* halt on OOM */
            }
            t_cache = tmp;
            cache_cap = new_cap;
        }
        t_cache[cache_len] = nt;
        cache_len++;

        /* update state for next iteration */
        x = nx; y = ny; z = nz; t = nt; u = nu; v = nv;
    }

    return t_cache[(size_t)n];
}

/*
 * A210466  Length of the shortest path to the hardest to obtain volumes in
 *          the two jugs problem.
 * https://oeis.org/A210466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the length of the shortest solution to the “two jugs
 *            problem” for the n‑th coprime pair (x,y) with x>y>1, enumerated
 *            in increasing x and then y
 * Range:     u8 n <= 5023, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 84 terms through n = 8460 (2026-11-01)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function computes the maximum over all goal volumes
   1..x of the minimal number of steps needed to reach that
   volume with the jugs (x,y).  */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/gcd.h"

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

static seq_t compute_max_steps(seq_t X, seq_t Y)
{
    /* Ensure X ≥ Y */
    if (X < Y) { seq_t t = X; X = Y; Y = t; }

    idx_t a = (idx_t)X;  /* larger jug capacity */
    idx_t b = (idx_t)Y;  /* smaller jug capacity */

    /* BFS over all states (ci,cj) with 0≤ci≤a, 0≤cj≤b */
    idx_t states = (a + 1) * (b + 1);
    idx_t *dist = (idx_t*)calloc(states, sizeof(idx_t));
    if (!dist) abort();
    idx_t *queue = (idx_t*)malloc(states * sizeof(idx_t));
    if (!queue) { free(dist); abort(); }

    /* initialise dist[] with INF */
    const idx_t INF = (idx_t)-1;
    for (idx_t i = 0; i < states; ++i) dist[i] = INF;

    dist[0] = 0;                 /* start state (0,0) has distance 0 */
    idx_t head = 0, tail = 1;
    queue[0] = 0;

    idx_t max_steps = 0;
    idx_t *fnd = (idx_t*)calloc(a + 1, sizeof(idx_t));
    if (!fnd) { free(dist); free(queue); abort(); }
    idx_t found = 0;

    while (head < tail)
    {
        idx_t cur = queue[head++];
        idx_t curdist = dist[cur];
        idx_t ci = cur / (b + 1);   /* amount in larger jug */
        idx_t cj = cur % (b + 1);   /* amount in smaller jug */

        /* record first time each goal volume is reachable */
        if (ci > 0 && !fnd[ci])
        {
            fnd[ci] = 1;
            if (curdist > max_steps) max_steps = curdist;
            if (++found == a) break;
        }
        if (cj > 0 && !fnd[cj])
        {
            fnd[cj] = 1;
            if (curdist > max_steps) max_steps = curdist;
            if (++found == a) break;
        }

        /* 1. Fill larger jug */
        {
            idx_t nci = a;
            idx_t ncj = cj;
            idx_t nidx = nci * (b + 1) + ncj;
            if (dist[nidx] == INF) { dist[nidx] = curdist + 1; queue[tail++] = nidx; }
        }
        /* 2. Fill smaller jug */
        {
            idx_t nci = ci;
            idx_t ncj = b;
            idx_t nidx = nci * (b + 1) + ncj;
            if (dist[nidx] == INF) { dist[nidx] = curdist + 1; queue[tail++] = nidx; }
        }
        /* 3. Empty larger jug */
        {
            idx_t nci = 0;
            idx_t ncj = cj;
            idx_t nidx = nci * (b + 1) + ncj;
            if (dist[nidx] == INF) { dist[nidx] = curdist + 1; queue[tail++] = nidx; }
        }
        /* 4. Empty smaller jug */
        {
            idx_t nci = ci;
            idx_t ncj = 0;
            idx_t nidx = nci * (b + 1) + ncj;
            if (dist[nidx] == INF) { dist[nidx] = curdist + 1; queue[tail++] = nidx; }
        }
        /* 5. Pour larger → smaller */
        {
            idx_t delta = (ci <= (b - cj)) ? ci : (b - cj);
            idx_t nci = ci - delta;
            idx_t ncj = cj + delta;
            idx_t nidx = nci * (b + 1) + ncj;
            if (dist[nidx] == INF) { dist[nidx] = curdist + 1; queue[tail++] = nidx; }
        }
        /* 6. Pour smaller → larger */
        {
            idx_t delta = (cj <= (a - ci)) ? cj : (a - ci);
            idx_t nci = ci + delta;
            idx_t ncj = cj - delta;
            idx_t nidx = nci * (b + 1) + ncj;
            if (dist[nidx] == INF) { dist[nidx] = curdist + 1; queue[tail++] = nidx; }
        }
    }

    free(dist);
    free(queue);
    free(fnd);

    return (seq_t)max_steps;
}

/* ----------  cache and enumeration logic  ---------- */

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t next_x = 3;
static idx_t next_y = 2;
static bool initialized = false;

static void cache_ensure(size_t new_len)
{
    if (new_len <= cache_cap) return;
    size_t new_cap = cache_cap ? (cache_cap << 1) : 16;
    if (new_cap < new_len) new_cap = new_len;
    seq_t *tmp = (seq_t*)seq_realloc(cache, cache_cap, new_cap);
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

static void advance_pair(void)
{
    ++next_y;
    if (next_y >= next_x) { ++next_x; next_y = 2; }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for offset 1 */

    if (!initialized)
    {
        initialized = true;
        next_x = 3;
        next_y = 2;
        cache_len = 0;
        cache_cap = 0;
        cache = NULL;
    }

    /* generate terms until we have at least n of them */
    while (cache_len < (size_t)n)
    {
        /* skip non–coprime pairs */
        while (gcd((seq_t)next_x, (seq_t)next_y) != (seq_t)1)
            advance_pair();

        seq_t val = compute_max_steps((seq_t)next_x, (seq_t)next_y);

        cache_ensure(cache_len + 1);
        cache[cache_len++] = val;

        advance_pair();   /* move to the next pair for the next loop */
    }

    return cache[cache_len - 1];
}

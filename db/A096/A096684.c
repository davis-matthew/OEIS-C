/*
 * A096684  Least k such that decimal representation of k*n contains only
 *          digits 0 and 5.
 * https://oeis.org/A096684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = A078244(n)/n where A078244(n) is the least multiple of n
 *            whose decimal digits are only 0 or 5. The function returns a(n)
 *            defined for n >= 1
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;                       /* undefined behaviour for n==0 */

    /*_{ (n) is an idx_t, but we need it for array sizes, which are size_t ) }
       The maximum n for which we reasonably expect this routine is small
       (e.g. n <= 10000), so the allocation below is fine. */
    size_t mod = (size_t)n;

    /* Predecessor and digit used to reach each remainder. */
    size_t *prev  = (size_t*)malloc(mod * sizeof(size_t));
    unsigned char *digit = (unsigned char*)malloc(mod * sizeof(unsigned char));
    bool *vis = (bool*)malloc(mod * sizeof(bool));
    size_t *queue = (size_t*)malloc(mod * sizeof(size_t));

    for (size_t i = 0; i < mod; i++) vis[i] = false;

    /* BFS queue indices */
    size_t qh = 0, qt = 0;

    size_t start_rem = 5 % mod;
    vis[start_rem] = true;
    prev[start_rem] = SIZE_MAX;   /* sentinel for root */
    digit[start_rem] = 5;
    queue[qt++] = start_rem;

    size_t target = SIZE_MAX;
    if (start_rem == 0) {
        target = 0;
    } else {
        /* Breadth‑first search for remainder 0 */
        while (qh < qt && target == SIZE_MAX) {
            size_t r = queue[qh++];
            unsigned char d;
            for (d = 0; d < 6; d += 5) {          /* try digits 0 and 5 */
                size_t nr = (r * 10 + d) % mod;
                if (!vis[nr]) {
                    vis[nr] = true;
                    prev[nr] = r;
                    digit[nr] = d;
                    queue[qt++] = nr;
                    if (nr == 0) {
                        target = 0;                /* found the required multiple */
                        break;
                    }
                }
            }
        }
    }

    /* Reconstruct the digits of the minimal multiple (from most to least
       significant) */
    size_t r = target;
    size_t len = 0;
    unsigned char *path = (unsigned char*)malloc(mod * sizeof(unsigned char));
    while (r != SIZE_MAX) {
        path[len++] = digit[r];
        r = prev[r];
    }
    /* The path is currently least→most significant, reverse it. */
    for (size_t i = 0; i < len / 2; ++i) {
        unsigned char tmp = path[i];
        path[i] = path[len - 1 - i];
        path[len - 1 - i] = tmp;
    }

    /* Divide the constructed number by n, building the quotient (k) digit by
       digit.  No large intermediate number is needed. */
    uint64_t rem = 0ULL;
    seq_t k_val = 0;
    bool started = false;
    for (size_t i = 0; i < len; ++i) {
        rem = rem * 10 + path[i];
        uint64_t q = rem / (uint64_t)n;
        rem = rem % (uint64_t)n;
        if (!started) {
            if (q != 0) {
                started = true;
                k_val = (seq_t)q;
            }
        } else {
            k_val = k_val * 10 + (seq_t)q;
        }
    }

    /* Clean up */
    free(prev);
    free(digit);
    free(vis);
    free(queue);
    free(path);

    return k_val;
}

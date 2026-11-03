/*
 * A078250  a(1) = 1, a(n) = smallest multiple of n using prime digits if n is
 *          composite else smallest multiple of n using composite digits, with
 *          a(n) = 0 if there are none.
 * https://oeis.org/A078250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = smallest multiple of n whose decimal digits are prime
 *            (2,3,5,7) if n is composite, composite (0,4,6,8,9) if n is
 *            prime, leading digit forbidden from 0, a(1) = 1. If no such
 *            multiple exists, return 0
 * Range:     u8 n <= 12, u16 n <= 181, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 9387 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses a breadth‑first search over the
   remainders modulo n to find the minimal digit‑restricted multiple.
   The search is guaranteed to terminate because there are only n
   possible remainders; if none lead to 0 we return 0.  */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Breadth‑first search over remainders.  digits must contain all
   allowed digits in ascending order, start_digits the allowed
   leading digits (no zero), and their sizes.  */
static seq_t bfs_min_multiple(idx_t n,
                              const int *digits, int ndigits,
                              const int *start_digits, int nstart)
{
    /* per‑remainder data */
    bool *vis = (bool *)calloc((size_t)n, sizeof(bool));
    idx_t *prev = (idx_t *)malloc((size_t)n * sizeof(idx_t));
    int   *prev_d = (int   *)malloc((size_t)n * sizeof(int));

    /* simple circular queue */
    int *queue = (int *)malloc((size_t)n * sizeof(int));
    int head = 0, tail = 0;

    /* initialise with one‑digit numbers */
    for (int i = 0; i < nstart; ++i) {
        int d = start_digits[i];
        idx_t r = d % n;
        if (!vis[r]) {
            vis[r] = true;
            prev[r] = (idx_t)-1;       /* no predecessor */
            prev_d[r] = d;
            queue[tail++] = (int)r;
            if (r == 0) {              /* single‑digit solution */
                free(vis); free(prev); free(prev_d); free(queue);
                seq_t res = 0;
                res = res * 10 + d;
                return res;
            }
        }
    }

    /* BFS loop */
    while (head < tail) {
        int cur = queue[head++];
        for (int k = 0; k < ndigits; ++k) {
            int d = digits[k];
            idx_t nr = (cur * 10 + d) % n;
            if (!vis[nr]) {
                vis[nr] = true;
                prev[nr] = (idx_t)cur;
                prev_d[nr] = d;
                queue[tail++] = (int)nr;
                if (nr == 0) {          /* solution found */
                    /* reconstruct digits backwards */
                    int buf[64];         /* n ≤ 10⁴ → < 64 digits in practice */
                    int len = 0;
                    int r = (int)nr;
                    while (r != (int)-1) {
                        buf[len++] = prev_d[r];
                        r = prev[r];
                    }
                    /* reverse to correct order */
                    for (int i = 0; i < len / 2; ++i) {
                        int tmp = buf[i];
                        buf[i] = buf[len - 1 - i];
                        buf[len - 1 - i] = tmp;
                    }
                    /* build the number */
                    seq_t res = 0;
                    for (int i = 0; i < len; ++i)
                        res = res * 10 + (seq_t)buf[i];
                    free(vis); free(prev); free(prev_d); free(queue);
                    return res;
                }
            }
        }
    }

    /* no multiple exists */
    free(vis); free(prev); free(prev_d); free(queue);
    return 0;
}

typedef seq_t seq_t;
typedef idx_t idx_t;

/* Simple deterministic primality test for n ≤ 10^8  */
static bool is_prime_u64(idx_t n)
{
    if (n < 2) return false;
    if (n == 2 || n == 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (idx_t i = 5; i * i <= n; i += 6)
        if (n % i == 0 || n % (i + 2) == 0)
            return false;
    return true;
}

seq_t a(idx_t n)
{
    if (n == 1) return 1;

    /* digit sets */
    static const int prime_digits[]      = {2,3,5,7};
    static const int composite_digits[]   = {0,4,6,8,9};
    static const int composite_start[]    = {4,6,8,9};

    if (is_prime_u64(n)) {
        /* prime: use composite digits */
        return bfs_min_multiple(n,
                                composite_digits,
                                sizeof(composite_digits)/sizeof(int),
                                composite_start,
                                sizeof(composite_start)/sizeof(int));
    } else {
        /* composite: use prime digits */
        return bfs_min_multiple(n,
                                prime_digits,
                                sizeof(prime_digits)/sizeof(int),
                                prime_digits,
                                sizeof(prime_digits)/sizeof(int));
    }
}

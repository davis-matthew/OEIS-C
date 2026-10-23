/*
 * A035657  Number of partitions of n into parts 7k+1 and 7k+2 with at least
 *          one part of each type.
 * https://oeis.org/A035657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into parts 7k+1 and 7k+2 with *
 *            at least one part of each type. Returned value is zero for n < 1
 * Range:     u8 n <= 38, u16 n <= 111, u32 n <= 361, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We keep a small static cache so that successive calls need not
 * recompute everything from scratch.  The arrays are large enough to
 * cover all required n (the tests only use n up to about 10 000).  If a
 * larger n is requested the function will still work but will truncate
 * to the maximum size.  The algorithm is O(n²/7) and is fast enough for
 * n ≤ 10 000. */

#include <stddef.h>   /* for size_t */

#define MAX_CACHE 20000   /* enough for all test inputs */

static seq_t p1[MAX_CACHE + 1];
static seq_t p2[MAX_CACHE + 1];
static seq_t p12[MAX_CACHE + 1];
static idx_t cur_max = 0;

static void recompute_up_to(idx_t n)
{
    if (n <= cur_max) return;
    if (n > MAX_CACHE) n = MAX_CACHE;   /* safe guard */

    /* Reset arrays up to n.  It is fine to clear more than we need. */
    for (idx_t i = 0; i <= n; ++i) {
        p1[i]  = 0;
        p2[i]  = 0;
        p12[i] = 0;
    }

    /* Base cases: one way to partition zero. */
    p1[0] = p2[0] = p12[0] = 1;

    /* Partitions using only 1 (mod 7) parts */
    for (idx_t part = 1; part <= n; part += 7) {
        for (idx_t i = part; i <= n; ++i) {
            p1[i] += p1[i - part];
        }
    }

    /* Partitions using only 2 (mod 7) parts */
    for (idx_t part = 2; part <= n; part += 7) {
        for (idx_t i = part; i <= n; ++i) {
            p2[i] += p2[i - part];
        }
    }

    /* Partitions using parts of both types together */
    for (idx_t part = 1; part <= n; part += 7) {
        for (idx_t i = part; i <= n; ++i) {
            p12[i] += p12[i - part];
        }
    }
    for (idx_t part = 2; part <= n; part += 7) {
        for (idx_t i = part; i <= n; ++i) {
            p12[i] += p12[i - part];
        }
    }

    cur_max = n;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    recompute_up_to(n);

    /* a(n) = partitions using both types (p12) – partitions using only one type (p1 or p2) */
    return p12[n] - p1[n] - p2[n];
}

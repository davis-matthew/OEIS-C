/*
 * A191352  Numbers n with property that 9*n is a sum of two distinct positive
 *          cubes.
 * https://oeis.org/A191352
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 9935, u64 n <= 9935, u128 n <= 9935,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A191352 – n such that 9*n = a³ + b³ with distinct positive a,b.
   The algorithm enumerates all unordered pairs a<b up to a fixed bound,
   keeps the values n whose cubic sum is divisible by 9, sorts and
   deduplicates them, and serves the result for any requested index. */
 
#include <stdlib.h>
#include <stdbool.h>

static seq_t *seq_values = NULL; /* sorted unique n values */
static size_t seq_count  = 0;     /* number of terms stored */
static bool seq_ready    = false;

#ifdef __cplusplus
#include <vector>
#include <algorithm>
static void build_sequence(void)
{
    const idx_t MAX_A = 800;                 /* enough for > 19000 terms */
    std::vector<seq_t> tmp;
    tmp.reserve((size_t)MAX_A * (MAX_A - 1) / 2);

    for (idx_t a = 1; a < MAX_A; ++a) {
        seq_t a3 = (seq_t)a * a * a;
        for (idx_t b = a + 1; b < MAX_A; ++b) {
            seq_t s = a3 + (seq_t)b * b * b;
            if ((s % 9) == 0)
                tmp.push_back(s / 9);                 /* n = (a³+b³)/9 */
        }
    }

    if (tmp.empty())
        return;

    std::sort(tmp.begin(), tmp.end());
    auto last = std::unique(tmp.begin(), tmp.end());
    seq_count  = last - tmp.begin();
    seq_values = new seq_t[seq_count];
    for (size_t i = 0; i < seq_count; ++i)
        seq_values[i] = tmp[i];
    seq_ready = true;
}
#else  /* C implementation */
static int cmp_seq(const void *p1, const void *p2)
{
    const seq_t *x = (const seq_t *)p1;
    const seq_t *y = (const seq_t *)p2;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

static void build_sequence(void)
{
    const idx_t MAX_A = 800;                               /* enough for > 19000 terms */
    size_t capacity = (size_t)MAX_A * (MAX_A - 1) / 2;
    seq_t *tmp = (seq_t *)malloc(capacity * sizeof(seq_t));
    if (!tmp) return;                                      /* allocation failure */

    size_t tcnt = 0;
    for (idx_t a = 1; a < MAX_A; ++a) {
        seq_t a3 = (seq_t)a * a * a;
        for (idx_t b = a + 1; b < MAX_A; ++b) {
            seq_t s = a3 + (seq_t)b * b * b;
            if ((s % 9) == 0)
                tmp[tcnt++] = s / 9;                         /* n = (a³+b³)/9 */
        }
    }

    if (tcnt == 0) {
        free(tmp);
        return;
    }

    qsort(tmp, tcnt, sizeof(seq_t), cmp_seq);

    /* Count distinct values */
    seq_values = (seq_t *)malloc(tcnt * sizeof(seq_t));
    if (!seq_values) {
        free(tmp);
        return;
    }
    seq_count = 1;
    seq_values[0] = tmp[0];
    for (size_t i = 1; i < tcnt; ++i)
        if (tmp[i] != tmp[i - 1])
            seq_values[seq_count++] = tmp[i];

    free(tmp);
    seq_ready = true;
}
#endif

seq_t a(idx_t n)
{
    if (!seq_ready)
        build_sequence();
    if (n == 0 || n > seq_count)
        return 0;                /* out‑of‑range or undefined behaviour */
    return seq_values[n - 1];
}

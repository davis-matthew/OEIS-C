/*
 * A085184  Sequence A085183 shown in base 4. Quaternary code for binary
 *          trees.
 * https://oeis.org/A085184
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th base‑4 number whose digits satisfy w(0) = –1,
 *            w(1) = w(2) = 0, w(3) = +1 with all prefixes having non‑negative
 *            cumulative sum and the final sum equal to 0. (The first three
 *            terms are 0, 1, 2 by definition.)
 * Range:     u8 n <= 18, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <limits.h>

static const int weight[4] = { -1, 0, 0, 1 };

/*  DP table:  g[len][bal] = number of admissible strings of length len
    that start with balance 'bal' and end with balance 0.  All values
    are stored as uint64_t; overflow is capped at UINT64_MAX.       */
static uint64_t **g = NULL;          /* g[0..max_len][0..len]   */
static uint64_t *count_len = NULL;   /* count_len[len] = g[len][0] */
static size_t max_len = 0;

/* build DP tables up to 'target_len' */
static void ensure_len(size_t target_len)
{
    if (target_len <= max_len) return;

    /* allocate new tables */
    uint64_t **new_g = (uint64_t **)malloc((target_len + 1) * sizeof(uint64_t *));
    if (!new_g) exit(1);
    for (size_t i = 0; i <= target_len; ++i) new_g[i] = NULL;

    new_g[0] = (uint64_t *)malloc(sizeof(uint64_t));
    new_g[0][0] = 1;                    /* g[0][0] = 1 */

    for (size_t l = 1; l <= target_len; ++l) {
        new_g[l] = (uint64_t *)malloc((l + 1) * sizeof(uint64_t));
        for (size_t bal = 0; bal <= l; ++bal) {
            unsigned __int128 val = 0;
            if (bal >= 1)                 /* digit 0 */
                val += new_g[l - 1][bal - 1];
            val += (__int128)new_g[l - 1][bal] * 2;   /* digits 1 and 2 */
            if (bal + 1 <= l - 1)         /* digit 3 */
                val += new_g[l - 1][bal + 1];
            if (val > (unsigned __int128)UINT64_MAX)
                val = UINT64_MAX;
            new_g[l][bal] = (uint64_t)val;
        }
    }

    /* counts of strings of each length */
    uint64_t *new_count = (uint64_t *)malloc((target_len + 1) * sizeof(uint64_t));
    for (size_t l = 0; l <= target_len; ++l)
        new_count[l] = new_g[l][0];

    /* free old tables */
    if (g) {
        for (size_t i = 1; i <= max_len; ++i) free(g[i]);
        free(g);
    }
    if (count_len) free(count_len);

    g = new_g;
    count_len = new_count;
    max_len = target_len;
}

/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)                /* not defined, return 0 to be safe */
        return 0;
    if (n == 1)                /* the special first term */
        return 0;
    if (n == 2)
        return 1;
    if (n == 3)
        return 2;

    /* shift index to exclude the first three special terms */
    idx_t offset = n - 3;      /* 1‑based index among strings of length >= 2 */

    /* find the length of the string */
    size_t len = 2;
    ensure_len(len);
    unsigned long long cum = 0;
    size_t L = 0;
    while (1) {
        if (len > max_len) ensure_len(len);
        cum += (unsigned long long)count_len[len];
        if (cum >= (unsigned long long)offset) {
            L = len;
            break;
        }
        ++len;
    }
    idx_t offset2 = offset - (idx_t)(cum - (unsigned long long)count_len[L]);

    /* generate the L‑digit string */
    seq_t  value = 0;
    int balance = 0;
    for (size_t pos = 0; pos < L; ++pos) {
        size_t rem = L - pos - 1;
        for (int d = 0; d < 4; ++d) {
            int nb = balance + weight[d];
            if (nb < 0) continue;
            uint64_t cnt = g[rem][nb];
            if ((uint64_t)offset2 > cnt) {
                offset2 -= (idx_t)cnt;
            } else {
                value = value * 10 + (seq_t)d;
                balance = nb;
                break;
            }
        }
    }
    return value;
}

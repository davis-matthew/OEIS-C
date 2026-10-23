/*
 * A031603  Numbers k such that the continued fraction for sqrt(k) has odd
 *          period and if the last term of the periodic part is deleted the
 *          two central terms are both 15.
 * https://oeis.org/A031603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A031603 – numbers k such that the continued fraction for sqrt(k)
 *            has an odd period and, after deleting the last term of that
 *            period, the two central terms are both 15
 * Range:     u8 none, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Integer square‑root (floor) using binary search */
static idx_t isqrt(idx_t n)
{
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        if ((__int128)mid * mid <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* Test whether a given k satisfies the period/central‑term condition */
static bool is_good(idx_t k)
{
    idx_t a0 = isqrt(k);
    if (a0 * a0 == k)                /* perfect square – skip */
        return false;

    idx_t m = 0, d = 1, a = a0;

    size_t cap = 16, len = 0;
    idx_t *period = (idx_t *)malloc(sizeof(idx_t) * cap);
    if (!period) return false;

    while (1) {
        m = d * a - m;
        __int128 m2 = (__int128)m * m;
        idx_t d_next = (idx_t)(((__int128)k - m2) / d);
        d = d_next;
        a = (a0 + m) / d;

        /* store the period term */
        if (len == cap) {
            cap <<= 1;
            period = (idx_t *)realloc(period, sizeof(idx_t) * cap);
            if (!period) return false;
        }
        period[len++] = a;

        if (a == 2 * a0)        /* period finished */
            break;
    }

    bool good = false;
    if (len >= 3 && (len & 1)) {           /* odd period with at least 3 terms */
        size_t i1 = (len - 1) / 2 - 1;     /* first central term */
        size_t i2 = (len - 1) / 2;         /* second central term */
        if (period[i1] == 15 && period[i2] == 15)
            good = true;
    }

    free(period);
    return good;
}

/* Sequence entry a(n) */
seq_t a(idx_t n)
{
    /* static storage for memoisation */
    static seq_t *list = NULL;
    static idx_t list_len = 0, list_cap = 0;
    static idx_t cur_k = 2;               /* next k to test */

    if (!list) {
        list_cap = 64;
        list = (seq_t *)malloc(sizeof(seq_t) * list_cap);
    }

    while (list_len < n) {
        while (!is_good(cur_k))
            cur_k++;

        if (list_len == list_cap) {
            list_cap <<= 1;
            list = (seq_t *)realloc(list, sizeof(seq_t) * list_cap);
        }
        list[list_len++] = (seq_t)cur_k;
        cur_k++;
    }

    return list[n - 1];
}

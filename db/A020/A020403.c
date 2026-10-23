/*
 * A020403  Numbers k such that the continued fraction for sqrt(k) has period
 *          64.
 * https://oeis.org/A020403
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th positive integer k whose continued‑fraction *
 *            expansion of sqrt(k) has period 64
 * Range:     u8 none, u16 n <= 616, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses a lazy sieve: it tests integers in
 * ascending order, stopping the period calculation as soon as the
 * length exceeds 64 to keep the routine fast. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Integer square root: floor(sqrt(n)).  Works for 0 <= n < 2^64. */
static inline idx_t _isqrt_u64(idx_t n) {
    idx_t low = 0;
    idx_t high = (idx_t)1 << 32;   /* > sqrt(2^64-1) */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if ((unsigned __int128)mid * mid <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* Return true if the continued‑fraction period of sqrt(d) equals 64.
 * The algorithm stops early as soon as the period exceeds 64. */
static inline bool _is_period64(idx_t d) {
    idx_t a0 = _isqrt_u64(d);
    if ((unsigned __int128)a0 * a0 == d)   /* perfect square */
        return false;

    idx_t m = 0;
    idx_t dd = 1;
    idx_t a  = a0;
    idx_t count = 0;

    while (true) {
        /* m_{n+1} = d_n * a_n - m_n */
        m = (idx_t)(((unsigned __int128)dd * a) - m);

        /* d_{n+1} = (d - m_{n+1}^2) / d_n */
        dd = (idx_t)(((unsigned __int128)d - ((unsigned __int128)m * m)) / dd);

        /* a_{n+1} = floor((a0 + m_{n+1}) / d_{n+1}) */
        a  = (a0 + m) / dd;

        ++count;
        if (a == 2 * a0)              /* end of period */
            break;
        if (count > 64)               /* period too long to be 64 */
            return false;
    }
    return count == 64;
}

/* Public sequence function.  It lazily builds the list of all k with
 * period 64, storing them in a static table.  The table grows as needed.
 * The function is thread‑safe for read‑only access after construction,
 * but construction itself is not protected by locks. */
seq_t a(idx_t n) {
    if (n == 0)  /* off‑by‑one safety; offset is 1 */
        return 0;

    static idx_t *table = NULL;   /* array of found k values (idx_t suffices) */
    static size_t tbl_len = 0;    /* how many terms already found */
    static size_t tbl_cap = 0;    /* current capacity */

    /* Grow table until we have at least n terms. */
    while (tbl_len < n) {
        /* Test next integer (skip perfect squares).  Start from 2 and
         * increase by one each call.  The static variable below remembers
         * where we left off. */
        static idx_t next_candidate = 2;
        idx_t d = next_candidate++;
        if (_is_period64(d)) {
            /* Resize if necessary. */
            if (tbl_len == tbl_cap) {
                size_t new_cap = tbl_cap ? tbl_cap * 2 : 4;
                idx_t *tmp = (idx_t *)realloc(table, new_cap * sizeof(idx_t));
                if (!tmp) exit(1);          /* out of memory */
                table = tmp;
                tbl_cap = new_cap;
            }
            table[tbl_len++] = d;
        }
    }

    /* Convert the idx_t value to seq_t before returning. */
    return (seq_t)table[n - 1];
}

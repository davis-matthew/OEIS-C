/*
 * A080745  Inverse Aronson transform of Fibonacci numbers 1,2,3,5,8,13,...
 * https://oeis.org/A080745
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n‑th element of the concatenation of segments
 *            defined by Fibonacci numbers (OEIS A080745)
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* maximum index that can be handled – large enough for n up to 10^4
   and well below 64‑bit overflow for Fibonacci numbers */
#define MAX_K 200

static seq_t fib[MAX_K + 2];
static idx_t cum[MAX_K + 1];
static bool is_fib_index[MAX_K + 2];

static idx_t fib_len = 2;   /* number of Fibonacci entries stored */
static idx_t cum_len = 0;   /* number of segments processed */
static bool   initialized = false;

seq_t a(idx_t n)
{
    /* === initialisation on first call === */
    if (!initialized) {
        fib[0]            = 0;
        fib[1]            = 1;
        fib_len           = 2;
        for (idx_t i = 0; i < MAX_K + 2; ++i) is_fib_index[i] = false;
        is_fib_index[0]   = true;
        is_fib_index[1]   = true;   /* 1 occurs twice in the Fibonacci sequence */
        cum[0]            = 0;
        cum_len           = 0;
        initialized       = true;
    }

    /* === extend the tables until the required cumulative count is reached === */
    while (cum_len == 0 || cum[cum_len] < n) {
        /* next segment index */
        idx_t k = cum_len + 1;

        /* ensure that fib[k] and fib[k+1] are available */
        while (fib_len <= k + 1) {
            fib[fib_len] = fib[fib_len - 1] + fib[fib_len - 2];
            if (fib[fib_len] < (seq_t)(MAX_K + 2))
                is_fib_index[(idx_t)fib[fib_len]] = true;
            fib_len++;
        }

        /* compute length of the k‑th segment */
        seq_t seg_len;
        if (k == 4) {
            /* special rule: segment 4 is empty */
            seg_len = 0;
        } else if (is_fib_index[k]) {
            seg_len = 1;
        } else {
            /* {F_k+1, …, F_{k+1}-1} */
            seg_len = fib[k + 1] - fib[k] - 1;
        }

        /* record cumulative count */
        cum_len++;
        cum[cum_len] = cum[cum_len - 1] + (idx_t)seg_len;
    }

    /* === locate the segment that contains the n‑th term === */
    idx_t k = 1;
    while (cum[k] < n) k++;

    idx_t      prev    = cum[k - 1];
    idx_t      offset  = n - prev - 1;   /* 0‑based offset inside the segment */

    /* compute the term */
    if (k == 4) {
        /* unreachable for valid n (segment 4 is empty) */
        return fib[4];
    }

    if (is_fib_index[k] && k != 4) {
        /* single element segment: {F_{k+1}} */
        return fib[k + 1];
    } else {
        /* range segment: {F_k+1, …, F_{k+1}-1} */
        return fib[k] + (seq_t)(offset + 1);
    }
}

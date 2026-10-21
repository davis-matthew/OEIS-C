/*
 * A219111  Numbers that eventually reach 1 under "x -> sum of 4th power of
 *          digits of x".
 * https://oeis.org/A219111
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   A219111 is the set of numbers that eventually reach 1 * under
 *            the map x → sum of the fourth powers of the digits of x. * The
 *            function enumerates numbers in increasing order and returns *
 *            the nth member of this sequence. The search uses a simple cycle
 *            * detection and processes numbers lazily as hi...
 * Range:     u8 n <= 1, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#if defined(__cplusplus)
# include <vector>
#endif

/* Pre–computed fourth powers of digits */
static const seq_t pow4[10] = {
    (seq_t)0,  (seq_t)1,  (seq_t)16,  (seq_t)81,
    (seq_t)256, (seq_t)625, (seq_t)1296, (seq_t)2401,
    (seq_t)4096, (seq_t)6561
};

/* Current number that the search will consider next */
static seq_t cur_check = 0;

/* Compute s(n) = sum of fourth powers of the decimal digits of n */
static seq_t digit_sum_fourth(seq_t n) {
    seq_t sum = 0;
    while (n > 0) {
        int d = (int)(n % 10);
        sum += pow4[d];
        n /= 10;
    }
    return sum;
}

/* Test whether a number eventually leads to 1.  Uses a fixed‑size
 * visited array for cycle detection.  The assumed maximum chain
 * length for the map x→digit_sum_fourth(x) is far below 200. */
static bool reaches1(seq_t start) {
    seq_t x = start;
    /* 200 steps are more than enough for all chains in this map. */
    static const int MAX_PATH = 200;
    seq_t path[MAX_PATH];
    int len = 0;
    while (1) {
        if (x == (seq_t)1)
            return true;

        /* cycle detection */
        for (int i = 0; i < len; ++i) {
            if (path[i] == x)
                return false;
        }

        if (len >= MAX_PATH)
            return false;   /* safety fallback */

        path[len++] = x;
        x = digit_sum_fourth(x);
    }
}

/* Storage for the sequence values.  We use a vector in C++ mode
 * and a malloc/realloc buffer in C mode. */
#if defined(__cplusplus)
static std::vector<seq_t> seq_values;
#else
static seq_t *seq_values = NULL;
static size_t seq_cap   = 0;
#endif
static size_t seq_len   = 0;   /* number of terms already computed */

/* Return the n-th term (1‑based) of the sequence.  The function
 * lazily generates further terms when a higher n is requested. */
seq_t a(idx_t n) {
    /* Initialisation guard */
    static bool init_done = false;
    if (!init_done) {
#if defined(__cplusplus)
        /* reserve some space to reduce reallocations */
        seq_values.reserve(2000);
#else
        seq_cap = 2048;
        seq_values = (seq_t *)malloc(seq_cap * sizeof(seq_t));
#endif
        init_done = true;
    }

    /* Generate more terms until we have at least n of them */
    while (seq_len < n) {
        /* Look for the next number that reaches 1 */
        do {
            ++cur_check;
            if (reaches1(cur_check)) {
#if defined(__cplusplus)
                seq_values.push_back(cur_check);
#else
                if (seq_len >= seq_cap) {
                    seq_cap *= 2;
                    seq_values = (seq_t *)realloc(seq_values,
                                                 seq_cap * sizeof(seq_t));
                }
                seq_values[seq_len] = cur_check;
#endif
                ++seq_len;
                break;
            }
        } while (1);
    }

    /* Return the requested term */
#if defined(__cplusplus)
    return seq_values[n - 1];
#else
    return seq_values[n - 1];
#endif
}

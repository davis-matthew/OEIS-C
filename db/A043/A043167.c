/*
 * A043167  Numbers k such that 2 and 5 occur juxtaposed in the base-8
 *          representation of k but not of k-1.
 * https://oeis.org/A043167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: a(n) is the n-th positive integer k such that
   the base‑8 representation of k contains the adjacent digits
   2 and 5 (in either order) while the base‑8 representation of k−1
   does not.  The sequence starts 21, 42, 85, … as given in the OEIS. */

#include <stdlib.h>
#include <stdbool.h>

static bool has_adjacent_25_52(seq_t k)
{
    /* sentinel larger than any octal digit */
    seq_t last = 8;
    while (k > 0) {
        seq_t d = k % 8;
        if (last != 8) {
            if ((last == 2 && d == 5) || (last == 5 && d == 2))
                return true;
        }
        last = d;
        k /= 8;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset is 1; guard for invalid input */

    static seq_t *seq   = NULL; /* cache of already found terms */
    static idx_t  count = 0;     /* number of terms stored   */
    static size_t capacity = 0;  /* allocated size of seq   */
    static seq_t next_k = 1;     /* next integer to test    */

    while (count < n) {
        seq_t k = next_k;

        /* search for the next k satisfying the definition */
        while (!(has_adjacent_25_52(k) && !has_adjacent_25_52(k - 1))) {
            ++k;
        }

        /* grow the cache if necessary */
        if (count == capacity) {
#if defined(__cplusplus)
            size_t new_cap = capacity ? capacity * 2 : 16;
            seq_t *tmp = new seq_t[new_cap];
            for (idx_t i = 0; i < count; ++i) tmp[i] = seq[i];
            delete[] seq;
            seq = tmp;
            capacity = new_cap;
#else
            size_t new_cap = capacity ? capacity * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
            if (!tmp) return 0;          /* allocation failed */
            seq = tmp;
            capacity = new_cap;
#endif
        }

        seq[count++] = k;
        next_k = k + 1;
    }
    return seq[n - 1];
}

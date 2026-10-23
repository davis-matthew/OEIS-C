/*
 * A043897  Numbers k such that 0 and 2 occur juxtaposed in the base-6
 *          representation of k but not of k+1.
 * https://oeis.org/A043897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth integer k such that the base‑6 representation of
 *            k contains an adjacent pair 0 and 2 (in either order) but the
 *            base‑6 representation of k+1 does not. The sequence is generated
 *            by a brute‑ force search and memoised in a static array
 * Range:     u8 n <= 14, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* make sure required types are visible when compiling */
#include <stddef.h>
#include <stdbool.h>

static bool has_02(seq_t x)
{
    if (x == 0)
        return false;
    seq_t six = 6;
    seq_t prev = x % six;
    x /= six;
    while (x > 0) {
        seq_t cur = x % six;
        if ((cur == 0 && prev == 2) || (cur == 2 && prev == 0))
            return true;
        prev = cur;
        x /= six;
    }
    return false;
}

seq_t a(idx_t n)
{
    static seq_t *list = NULL;       /* memoised terms */
    static size_t list_cap = 0;      /* capacity of list */
    static size_t list_len = 0;      /* number of stored terms */
    static idx_t last = 0;           /* last candidate examined */

    while (list_len < n) {
        ++last;                      /* next candidate */

        /* property check */
        if (has_02((seq_t)last) && !has_02((seq_t)(last + 1))) {
            /* grow storage if necessary */
            if (list_len == list_cap) {
                size_t new_cap = list_cap ? (list_cap << 1) : 16;
                seq_t *tmp = (seq_t *)realloc(list, new_cap * sizeof(seq_t));
                /* in a production build we would check tmp==NULL here */
                list = tmp;
                list_cap = new_cap;
            }
            list[list_len++] = (seq_t)last;
        }
    }

    return list[n - 1];
}

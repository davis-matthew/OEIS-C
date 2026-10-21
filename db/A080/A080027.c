/*
 * A080027  Let s(1) = 1; let s(2m) = {s(2m-1),m+1,s(2m-1)}, s(2m+1) =
 *          {s(2m),s(2m)}; sequence gives limit of s(k) for large k.
 * https://oeis.org/A080027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the limit of the sequence s(k) defined by s(1) = 1,
 *            s(2m) = s(2m-1) followed by (m+1) followed by s(2m-1), s(2m+1) =
 *            s(2m) followed by s(2m). The limit of the n‑th term as k→∞ is
 *            a(n)
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t get_even(idx_t n, idx_t m, const idx_t *lens);
static seq_t get_odd(idx_t n, idx_t m, const idx_t *lens);

seq_t a(idx_t n)
{
    /* Pre‑compute the lengths L(2m) until we reach or exceed n.
       L(2) = 3, L(2m) = 4*L(2m-2) + 1. */
    idx_t lens[128];
    lens[1] = 3;
    idx_t m = 1;
    while (lens[m] < n) {
        ++m;
        lens[m] = 4 * lens[m - 1] + 1;
    }

    return get_even(n, m, lens);
}

/* Return the value at position n (1‑based) in s(2m). */
static seq_t get_even(idx_t n, idx_t m, const idx_t *lens)
{
    if (m == 1) {          /* s(2) = [1, 2, 1] */
        switch (n) {
            case 1: return 1;
            case 2: return 2;
            default: return 1;   /* n == 3 */
        }
    }

    /* Lengths of the left part (s(2m‑1)) and the middle element. */
    idx_t len_prev   = lens[m - 1];          /* L(2m-2)  */
    idx_t left_len   = 2 * len_prev;         /* length of s(2m‑1) */
    idx_t mid_index  = left_len + 1;         /* position of m+1 */

    if (n == mid_index) 
        return (seq_t)(m + 1);              /* middle element */

    if (n <= left_len)                      /* left part: s(2m‑1) */
        return get_odd(n, m, lens);
    else                                    /* right part: another copy of s(2m‑1) */
        return get_odd(n - mid_index, m, lens);
}

/* Return the value at position n (1‑based) in s(2m‑1) which equals
   s(2m‑2) followed by s(2m‑2). */
static seq_t get_odd(idx_t n, idx_t m, const idx_t *lens)
{
    if (m == 1)              /* s(1) = [1] */
        return 1;

    idx_t len_prev = lens[m - 1];   /* length of s(2m-2) */

    if (n <= len_prev)
        return get_even(n, m - 1, lens);
    else
        return get_even(n - len_prev, m - 1, lens);
}

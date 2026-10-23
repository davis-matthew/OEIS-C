/*
 * A037349  Numbers whose base-3 and base-8 expansions have no digits in
 *          common.
 * https://oeis.org/A037349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generates the n-th term of OEIS A037349:
   Numbers whose base‑3 and base‑8 expansions have no digits in common.
   The sequence is obtained by brute‑force search. */

static int has_no_common_digits(idx_t v)
{
    unsigned mask3 = 0, mask8 = 0;
    idx_t t = v;
    while (t) {
        unsigned d = t % 3;
        mask3 |= 1u << d;
        t /= 3;
    }
    t = v;
    while (t) {
        unsigned d = t % 8;
        mask8 |= 1u << d;
        t /= 8;
    }
    return (mask3 & mask8) == 0;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t candidate = 1;
    while (count < n) {
        if (has_no_common_digits(candidate)) {
            ++count;
            if (count == n)
                return (seq_t)candidate;
        }
        ++candidate;
    }
    return (seq_t)0;  /* unreachable */
}

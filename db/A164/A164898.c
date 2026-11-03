/*
 * A164898  First differences of numbers having only odd digits in their
 *          decimal representation.
 * https://oeis.org/A164898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A014261(n+1) - A014261(n)
 * Range:     u8 n <= 279, u16 n <= 7029, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 111 terms through n = 9465 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Return the n-th number whose decimal digits are all odd.
   n is 1‑based. */
static seq_t odd_num(idx_t n)
{
    idx_t cumulative = 0;
    idx_t power = 5;          /* 5^1 */
    idx_t len = 1;

    /* Find the length of the desired number */
    while (cumulative + power < n) {
        cumulative += power;
        power *= 5;
        ++len;
    }

    idx_t offset = n - cumulative - 1;   /* 0‑based within this length class */

    /* Pre‑compute powers of 5 up to len */
    idx_t pow5[64];
    pow5[0] = 1;
    for (idx_t i = 1; i <= len; ++i)
        pow5[i] = pow5[i-1] * 5;

    seq_t num = 0;
    for (idx_t i = 0; i < len; ++i) {
        idx_t digit_index = offset / pow5[len-1-i];
        offset %= pow5[len-1-i];
        seq_t digit = (seq_t)(digit_index * 2 + 1);
        num = num * 10 + digit;
    }
    return num;
}

seq_t a(idx_t n)
{
    seq_t prev = odd_num(n);
    seq_t next = odd_num(n + 1);
    return next - prev;
}

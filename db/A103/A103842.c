/*
 * A103842  Triangle read by rows: row n is binary expansion of 2^n-n, n >= 1.
 * https://oeis.org/A103842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for sequence A103842.
   a(n) is the n-th bit in the concatenation of the rows
   R_r = binary(2^r - r) for r = 1,2,3…,
   where each row R_r contains r bits.
   The implementation finds the row r such that
   r(r+1)/2 >= n and then returns
   a(n) = 1 – bit_at_position((r-1), r-1 - (n - (r-1)*r/2 - 1)).  */

seq_t a(idx_t n)
{
    /* Handle out‑of‑range indices gracefully. */
    if (n < 1)
        return (seq_t)0;

    /* Find r: the unique integer with r(r+1)/2 >= n. */
    idx_t low = 1, high = 1;
    while (((unsigned __int128)high * (high + 1)) / 2 < n)
        high <<= 1;          /* double until the bound is sufficient */

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (((unsigned __int128)mid * (mid + 1)) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;              /* row number */

    /* Number of bits before this row. */
    idx_t cum_prev = ((unsigned __int128)(r - 1) * r) / 2;

    /* Zero‑based index within the row (left to right). */
    idx_t pos = n - cum_prev - 1;             /* 0 <= pos < r */

    /* Bit position counted from the least significant bit. */
    idx_t shift = (r - 1) - pos;              /* 0 <= shift < r */

    /* The bit is the complement of the bit of (r-1) at that position. */
    unsigned int b;
    if (shift < 64) {
        b = ((unsigned __int128)(r - 1) >> shift) & 1;
        b = 1 - b;            /* complement */
    } else {
        /* For shift >= 64, (r-1) has no such bit set, so complement is 1. */
        b = 1;
    }
    return (seq_t)b;
}

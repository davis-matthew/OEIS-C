/*
 * A092543  Table below read by antidiagonals alternately upwards and
 *          downwards.
 * https://oeis.org/A092543
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac, tabl
 * Formula:   not stated
 * Range:     u8 n <= 2043, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is obtained by reading the column indices of a table whose
 * every row is 1,2,3,… along antidiagonals that alternate between
 * upward and downward.  Let D be the smallest integer ≥2 such that
 * D*(D-1)/2 ≥ n.  Then
 *
 *   total_before = (D-2)*(D-1)/2            (elements before diagonal D)
 *   pos          = n - total_before - 1    (0‑based index inside diagonal D)
 *
 * If D is even (upward reading) then a(n) = pos + 1;
 * otherwise (D odd, downward reading) a(n) = (D-1) - pos.
 */
seq_t a(idx_t n)
{
    if (n == 0) {
        /* sequence is offset 1; undefined for n==0, return 0 */
        return (seq_t)0;
    }

    /* Find diagonal D such that D*(D-1)/2 >= n */
    idx_t low = 1;
    idx_t high = 2;
    while (((seq_t)high * (high - 1)) / 2 < (seq_t)n) {
        high <<= 1;                    /* double until high is large enough */
    }
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (((seq_t)mid * (mid - 1)) / 2 >= (seq_t)n) {
            high = mid;
        } else {
            low = mid + 1;
        }
    }
    idx_t D = low;                     /* D >= 2 */

    seq_t total_before = ((seq_t)(D - 2) * (D - 1)) / 2;
    seq_t pos = (seq_t)n - total_before - 1;   /* position inside diagonal */

    seq_t result;
    if ((D & 1) == 0) {                 /* even D => upward direction */
        result = pos + 1;
    } else {                            /* odd D => downward direction */
        result = (seq_t)(D - 1) - pos;
    }
    return result;
}

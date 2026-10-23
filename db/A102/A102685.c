/*
 * A102685  Partial sums of A055640.
 * https://oeis.org/A102685
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 128, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A102685: Total number of non‑zero digits occurring in all numbers 0,1,2,…,n
 *
 * Formula: a(n) = Σ_{j≥0} [ n/(10^{j+1}) * 9 * 10^j +
 *              (cur_j > 0 ? (cur_j-1) * 10^j + (low_j + 1) : 0 ) ]
 * where cur_j = (n / 10^j) % 10 and low_j = n % 10^j.
 *
 * The function below computes the sum in O(log10 n) time.
 */

seq_t a(idx_t n)
{
    seq_t total = 0;
    idx_t pow   = 1;           /* 10^j  (j = 0,1,2,…) */

    while (pow <= n) {
        /* --------- compute high, cur, low for this position --------- */
        idx_t high;
        if (pow > n / 10)          /* would overflow pow*10 */
            high = 0;
        else
            high = n / (pow * 10); /* full blocks of size 10^(j+1) */

        idx_t cur = (n / pow) % 10;
        idx_t low = n % pow;

        /* --------- count non‑zero digits contributed by this position --------- */
        seq_t in_full = (seq_t)high * 9 * (seq_t)pow;   /* full blocks */

        seq_t in_rem = 0;
        if (cur > 0) {
            in_rem = (seq_t)(cur - 1) * (seq_t)pow + (seq_t)(low + 1);
        }

        total += in_full + in_rem;

        /* --------- next decimal position --------- */
        if (pow > n / 10)   /* next multiplication would overflow or exceed n */
            break;
        pow *= 10;
    }

    return total;
}

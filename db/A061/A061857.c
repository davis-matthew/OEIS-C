/*
 * A061857  Triangle in which the k-th item in the n-th row (both starting
 *          from 1) is the number of ways in which we can add 2 distinct
 *          integers from 1 to n in such a way that the sum is divisible by k.
 * https://oeis.org/A061857
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Triangle entry a(n,k) computed by Let n+1 = b (mod k), 0 ≤ b <
 *            k, q = (n+1-b)/k Let c = k mod 2. If b == 0 or 1: a = (q²*k +
 *            c*q)/2 + q*b - 2*q - b + 1 Else if b ≥ (k+3)/2: a = (q²*k +
 *            c*(q+1) - k)/2 + q*b - 2*q + b - 1 Else: a = (q²*k + c*q)/2 +
 *            q*b - 2*q The public function returns the flattene...
 * Range:     u8 n <= 276, u16 n <= 9980, u32 n <= 9980, u64 n <= 9980,
 *            u128 n <= 9980, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the trivial case */
    if (n == 0u) return 0;

    /* Step 1: find the row r such that r*(r+1)/2 ≥ n */
    idx_t low = 1, high = 1;
    while ((unsigned __int128)high * (high + 1) / 2 < n)
        high <<= 1;                /* double until the bound is sufficient */

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((unsigned __int128)mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;                         /* row index (1‑based) */
    unsigned __int128 t_prev = (unsigned __int128)(r - 1) * r / 2;
    idx_t k = (idx_t)(n - t_prev);          /* column index (1‑based) */

    /* Step 2: compute the triangle entry using the formula */
    /* Use signed 128‑bit integers for intermediate arithmetic */
    __int128 R = r;
    __int128 K = k;

    __int128 n_plus_one = R + 1;
    __int128 b = n_plus_one % K;
    __int128 q = (n_plus_one - b) / K;
    __int128 c = K & 1;          /* parity of k */

    __int128 termA;          /* the part divided by two in the formula */
    __int128 rest;           /* remaining integer terms */

    if (b == 0 || b == 1) {
        termA = (q * q * K + c * q) / 2;
        rest   = q * b - 2 * q - b + 1;
    } else if (b >= (K + 3) / 2) {
        termA = (q * q * K + c * (q + 1) - K) / 2;
        rest   = q * b - 2 * q + b - 1;
    } else { /* 2 ≤ b < (k+3)/2 */
        termA = (q * q * K + c * q) / 2;
        rest   = q * b - 2 * q;
    }

    __int128 result_int = termA + rest;
    /* The result is known to be non‑negative for all inputs */
    return (seq_t)result_int;
}

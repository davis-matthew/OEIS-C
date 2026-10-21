/*
 * A055833  T(n,n-6), where T is the array in A055830.
 * https://oeis.org/A055833
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-5)*(n-4)*(n^3 + 19*n^2 + 6*n - 156)/120
 * Range:     u8 n <= 8, u16 n <= 23, u32 n <= 218, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A055833, valid for n >= 6.
   Uses 128‑bit arithmetic for intermediate results to avoid overflow
   when builtin unsigned long long is insufficient. */
seq_t a(idx_t n)
{
    /* For n < 6 the sequence is not defined; return 0 (behaviour irrelevant) */
    if (n < 6)
        return 0;

    /* Promote to 128‑bit for safe multiplication */
    unsigned __int128 N = n;

    /* Compute the prefactor (n-5)*(n-4) */
    unsigned __int128 pref = (N - 5) * (N - 4);

    /* Compute the cubic term n^3 + 19n^2 + 6n - 156 */
    unsigned __int128 cubic = N * N * N
                           + 19 * N * N
                           + 6 * N
                           - 156;

    /* Full numerator */
    unsigned __int128 num = pref * cubic;

    /* Divide by 120 (exact division) */
    unsigned __int128 res = num / 120;

    /* Cast back to seq_t (uint64_t, uint128_t, etc.) */
    return (seq_t)res;
}

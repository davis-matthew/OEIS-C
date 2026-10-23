/*
 * A138976  The discriminant of the characteristic polynomial of the O+ and O-
 *          submatrix for spin 3 of the nuclear electric quadrupole
 *          Hamiltonian is a perfect square for these values.
 * https://oeis.org/A138976
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 11(a(n-2) – a(n-4)) + a(n-6) (n ≥ 6) Initial values:
 *            a(0)=0, a(1)=-3, a(2)=-18, a(3)=-45, a(4)=-192, a(5)=-459
 * Range:     i8 n <= 3, i16 n <= 8, i32 n <= 18, i64 n <= 25, i128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Initial six terms of the sequence */
    seq_t cache[6] = {0, -3, -18, -45, -192, -459};

    /* For n < 6 the answer is already in cache */
    if (n < 6)
        return cache[n];

    /* Compute successively up to n using the linear recurrence */
    for (idx_t i = 6; i <= n; ++i) {
        cache[i % 6] =
            ((seq_t)11 * (cache[(i - 2) % 6] - cache[(i - 4) % 6])) + cache[(i - 6) % 6];
    }

    return cache[n % 6];
}

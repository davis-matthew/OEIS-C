/*
 * A205872  Numbers k for which 9 divides s(k)-s(j) for some j<k; each k
 *          occurs once for each such j; s(k) denotes the (k+1)-st Fibonacci
 *          number.
 * https://oeis.org/A205872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A205872
   a(n) = the smallest k such that the number of pairs (j,k),
   1 ≤ j < k, with F_{k+1} ≡ F_{j+1} (mod 9) first reaches n.
   The algorithm iterates k, keeping for each residue 0…8 the
   count of earlier indices producing that residue, and the
   cumulative count of such congruent pairs. */
seq_t a(idx_t n)
{
    /* Fibonacci residues modulo 9 for indices 1..24.
       The Pisano period for modulus 9 is 24, so the list repeats. */
    static const seq_t fib_mod9[24] = {
        1, 1, 2, 3, 5, 8, 4, 3, 7, 1, 8, 0,
        8, 8, 7, 6, 4, 1, 5, 6, 2, 8, 1, 0
    };

    /* Frequency of each residue among indices already processed. */
    static idx_t freq[9] = {0};
    /* Total number of congruent pairs processed so far. */
    static idx_t cumulative = 0;
    idx_t k = 1;

    /* Initialise counts for this independent call. */
    for (int r = 0; r < 9; ++r) freq[r] = 0;
    cumulative = 0;

    while (cumulative < n)
    {
        /* Index of F_{k+1} modulo 9 using the periodic table. */
        int idx = (int)((k + 1) % 24);
        if (idx == 0) idx = 24;                  /* map multiple of 24 to 24   */
        seq_t residue = fib_mod9[idx - 1];        /* 0..8 */

        /* Add the number of previous indices with the same residue. */
        cumulative += freq[(int)residue];

        /* Record appearance of this residue. */
        ++freq[(int)residue];

        ++k;
    }

    /* k has been incremented one step past the needed index. */
    return (seq_t)(k - 1);
}

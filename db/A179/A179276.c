/*
 * A179276  Largest 3-smooth number <= n.
 * https://oeis.org/A179276
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest 3‑smooth number ≤ n (i.e. of the form 2ᵃ·3ᵇ)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the sequence type once */
    seq_t N = (seq_t)n;
    seq_t best = 1;          /* at least 1 is always 3‑smooth */
    seq_t pow3 = 1;          /* current power of 3 */

    /* Iterate over powers of 3 that do not exceed N */
    while (pow3 <= N) {
        /* Maximum power of 2 that fits in N / pow3 */
        seq_t limit = N / pow3;
        seq_t pow2 = 1;
        while (pow2 <= limit / 2) {
            pow2 *= 2;
        }

        /* Candidate 2ⁱ·3ʲ ≤ N */
        seq_t cand = pow2 * pow3;
        if (cand > best) {
            best = cand;
        }

        /* Advance to the next power of 3 with overflow protection */
        if (pow3 > N / 3) {
            break;
        }
        pow3 *= 3;
    }

    return best;
}

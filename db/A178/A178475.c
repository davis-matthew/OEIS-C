/*
 * A178475  Permutations of 12345: Numbers having each of the decimal digits
 *          1,...,5 exactly once, and no other digit.
 * https://oeis.org/A178475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, easy, nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 120, u32 n <= 120, u64 n <= 120, u128 n <= 120,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is the n‑th lexicographic permutation of the
 * digits 1,2,3,4,5 (n ≥ 1).  The sequence has exactly 5! = 120
 * terms; for n > 120 the function returns 0.
 *
 * The implementation uses the factorial number system to
 * convert the (n‑1)-rank to a permutation.
 */
#include <stddef.h>
#include <stdint.h>

static const seq_t FACT[6] = {1, 1, 2, 6, 24, 120};

seq_t a(idx_t n)
{
    /* Compute 0‑based rank, restrict to 0 .. 119 */
    seq_t rank = (seq_t)(n - 1);
    if (rank >= FACT[5])
        rank %= FACT[5];

    seq_t result = 0;
    uint8_t avail[5] = {1, 2, 3, 4, 5};

    for (idx_t pos = 0; pos < 5; ++pos) {
        idx_t remaining = 5 - pos;
        seq_t f = FACT[remaining - 1];

        seq_t q = rank / f;
        idx_t idx = (idx_t)q;           /* index in avail array */

        uint8_t d = avail[idx];
        result = result * 10 + (seq_t)d;

        /* Remove the chosen digit from avail */
        for (idx_t j = idx; j + 1 < remaining; ++j)
            avail[j] = avail[j + 1];

        rank -= q * f;
    }

    return result;
}

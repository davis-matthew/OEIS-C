/*
 * A046034  Numbers whose digits are primes.
 * https://oeis.org/A046034
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n-th integer whose decimal digits are all prime
 *            digits (2,3,5,7), in increasing order. Implementation uses
 *            bijective base‑4 conversion: digits 1..4 in base‑4 correspond to
 *            prime digits 2,3,5,7 respectively
 * Range:     u8 n <= 31, u16 n <= 1108, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Mapping from bijective base‑4 digits (1..4) to prime digits */
    static const seq_t prime_map[5] = {0, 2, 3, 5, 7};

    /* Temporary storage for the mapped prime digits (least significant first) */
    seq_t mapped[64];
    int cnt = 0;

    idx_t temp = n;
    while (temp > 0) {
        /* Bijective base‑4: decrement before taking modulus */
        temp -= 1;
        unsigned digit = (unsigned)(temp % 4) + 1;   /* digit in 1..4 */
        mapped[cnt++] = prime_map[digit];
        temp /= 4;
    }

    seq_t result = 0;
    /* Construct the decimal number from most significant mapped digit to least significant */
    for (int i = cnt - 1; i >= 0; --i) {
        result = result * 10 + mapped[i];
    }

    return result;
}

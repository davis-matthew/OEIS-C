/*
 * A023733  Numbers with no 3's in base-5 expansion.
 * https://oeis.org/A023733
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = interpret (n‑1) in base‑4, mapping digits
 *            0→0,1→1,2→2,3→4, then reading the digits as a base‑5 number
 * Range:     u8 n <= 133, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const unsigned char _map4_to5[4] = {0, 1, 2, 4};

seq_t a(idx_t n) {
    idx_t tmp = n - 1;         /* we need n‑1 because a(1)=0 */
    seq_t result = 0;
    seq_t mult   = 1;          /* current power of 5 */

    while (tmp > 0) {
        unsigned d = tmp % 4;                    /* next digit in base‑4 */
        result += (seq_t)_map4_to5[d] * mult;    /* map to base‑5 digit and accumulate */
        mult *= 5;                               /* next power of 5 */
        tmp  /= 4;
    }
    return result;
}

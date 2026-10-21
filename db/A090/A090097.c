/*
 * A090097  Bases n such that the smallest prime-power-pseudoprime to base n
 *          is 9.
 * https://oeis.org/A090097
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A090097: bases n whose smallest prime‑power pseudoprime is 9.
   Computed by testing the congruence conditions:
      - n ≡ 1 or 8 (mod 9)
      - n ≠ 1,5 (mod 8)   (i.e. n even or n ≡ 3 or 7 (mod 8)).
   The function scans successive integers until the n‑th qualifying
   number is found. */

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t base  = 2;    /* start from 2; 1 never qualifies */

    while (1) {
        /* check modulo 9 requirement */
        idx_t m9 = base % 9;
        if ((m9 == 1 || m9 == 8)) {
            /* check modulo 8 requirement (exclude 1 and 5) */
            idx_t m8 = base % 8;
            if (m8 != 1 && m8 != 5) {
                ++count;
                if (count == n) {
                    return (seq_t)base;
                }
            }
        }
        ++base;
    }
}

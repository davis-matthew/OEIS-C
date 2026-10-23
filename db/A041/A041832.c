/*
 * A041832  Numerators of continued fraction convergents to sqrt(437).
 * https://oeis.org/A041832
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 13, u64 n <= 27, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A041832: Numerators of continued‑fraction convergents to √437
   Recurrence: a(n) = 9198*a(n-6) - a(n-12)
   Initial values: a(0)=20, a(1)=21, a(2)=209, a(3)=439, a(4)=4160, a(5)=4599,
                    a(6)=188120, a(7)=192719, a(8)=1922591, a(9)=4037901,
                    a(10)=38263700, a(11)=42301601 */
seq_t a(idx_t n)
{
    static const seq_t init[12] = {
        (seq_t)20,   (seq_t)21,   (seq_t)209,
        (seq_t)439,  (seq_t)4160, (seq_t)4599,
        (seq_t)188120, (seq_t)192719, (seq_t)1922591,
        (seq_t)4037901, (seq_t)38263700, (seq_t)42301601
    };

    /* Return initial terms directly. */
    if (n < 12) {
        return init[n];
    }

    /* Use a circular buffer of the last 12 computed values. */
    seq_t ring[12];
    for (idx_t i = 0; i < 12; ++i) {
        ring[i] = init[i];
    }

    /* Compute from a(12) up to a(n). */
    for (idx_t k = 12; k <= n; ++k) {
        /* Indices modulo 12 for ring buffer. */
        idx_t idx   = k % 12;
        idx_t idx6  = (k + 6) % 12;   /* k-6 mod 12 */
        idx_t idx12 = (k + 0) % 12;   /* k-12 mod 12, same as k mod 12 */

        /* Note: (k-6)%12 == (k+6)%12 because 12k ≡ 0 mod 12 */
        /* (k-12)%12 == k%12 */

        ring[idx] = (seq_t)9198 * ring[idx6] - ring[idx12];
    }

    return ring[n % 12];
}

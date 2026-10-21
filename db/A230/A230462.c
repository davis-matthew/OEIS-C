/*
 * A230462  Numbers congruent to {1, 11, 13, 17, 19, or 29} mod 30.
 * https://oeis.org/A230462
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 51, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A230462: numbers congruent to {1,11,13,17,19,29} mod 30.
   The terms satisfy:
   a(6k)   = 30k - 1
   a(6k-1) = 30k - 11
   a(6k-2) = 30k - 13
   a(6k-3) = 30k - 17
   a(6k-4) = 30k - 19
   a(6k-5) = 30k - 29
   (offset 1: a(1)=1). */
seq_t a(idx_t n)
{
    /* Since n ≥ 1 (offset 1), no need for special handling of 0. */
    seq_t k = (seq_t)(n / 6);          /* floor(n/6) */
    idx_t r = n % 6;                   /* n mod 6, takes values 0..5 */

    switch (r) {
        case 0: return (seq_t)30 * k - 1;
        case 1: return (seq_t)30 * (k + 1) - 29;
        case 2: return (seq_t)30 * (k + 1) - 19;
        case 3: return (seq_t)30 * (k + 1) - 17;
        case 4: return (seq_t)30 * (k + 1) - 13;
        case 5: return (seq_t)30 * (k + 1) - 11;
        default: return 0;  /* unreachable, but keeps compiler happy */
    }
}

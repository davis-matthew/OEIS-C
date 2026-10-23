/*
 * A185727  Integers of the form A145911(k)/(k+1) sorted along increasing k.
 * https://oeis.org/A185727
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = { n/6, if n mod 6 = 0; 9·⌊n/6⌋ + 2, if n mod 6 = 1;
 *            3·⌊n/6⌋ + 1, if n mod 6 = 2; 9·⌊n/6⌋ + 5, if n mod 6 = 3;
 *            3·⌊n/6⌋ + 2, if n mod 6 = 4; 9·⌊n/6⌋ + 8, if n mod 6 = 5; }
 * Range:     u8 n <= 170, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) for the OEIS sequence A185727 */
seq_t a(idx_t n)
{
    idx_t k = n / 6;          /* integer division gives floor(n/6) */
    idx_t r = n - 6 * k;      /* same as n % 6 but uses only integer ops */

    seq_t sk = (seq_t)k;      /* convert for use with seq_t arithmetic */
    seq_t res;

    switch (r) {
        case 0:  res = sk;                     break;
        case 1:  res = (9 * sk) + 2;           break;
        case 2:  res = (3 * sk) + 1;           break;
        case 3:  res = (9 * sk) + 5;           break;
        case 4:  res = (3 * sk) + 2;           break;
        case 5:  res = (9 * sk) + 8;           break;
        default: res = 0;                      break; /* unreachable */
    }

    return res;
}

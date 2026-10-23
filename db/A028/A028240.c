/*
 * A028240  Weight distribution of (256,2^16,120) Kerdock code.
 * https://oeis.org/A028240
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = coefficient of x^(256-8n) y^(8n) in the weight enumerator
 *            of the (256,2^16,120) Kerdock code. Only the weights 0, 120,
 *            128, 136 and 256 occur, corresponding to n = 0, 15, 16, 17 and
 *            32 respectively
 * Range:     u8 n <= 14, u16 n <= 32, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    switch (n) {
    case 0:  return (seq_t)1;     /* weight 0   */
    case 15: return (seq_t)32512; /* weight 120 */
    case 16: return (seq_t)510;   /* weight 128 */
    case 17: return (seq_t)32512; /* weight 136 */
    case 32: return (seq_t)1;     /* weight 256 */
    default: return (seq_t)0;     /* all other weights */
    }
}

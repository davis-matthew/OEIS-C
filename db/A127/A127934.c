/*
 * A127934  a(8n)=8n+1, a(8n+1)=a(8n+2)=a(8n+3)=8n+5, a(8n+4)=8n+6,
 *          a(8n+5)=a(8n+6)=a(8n+7)=8n+8.
 * https://oeis.org/A127934
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(8n)=8n+1, a(8n+1)=a(8n+2)=a(8n+3)=8n+5, a(8n+4)=8n+6,
 *            a(8n+5)=a(8n+6)=a(8n+7)=8n+8
 * Range:     u8 n <= 252, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t k = n / 8;
    idx_t r = n % 8;   /* remainder 0..7 */
    seq_t base = (seq_t)k * 8;
    switch (r) {
    case 0: return base + 1;
    case 1:
    case 2:
    case 3: return base + 5;
    case 4: return base + 6;
    case 5:
    case 6:
    case 7: return base + 8;
    default: return 0; /* unreachable */
    }
}

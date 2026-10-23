/*
 * A175919  Convert n to binary. XOR each respective digit of binary n and
 *          binary A030101(n), where A030101(n) is the reversal of the order
 *          of the digits in the binary representation of n (given in
 *          decimal). a(n) is the decimal value of the result.
 * https://oeis.org/A175919
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, look, nonn
 * Formula:   a(n) = A003987(n, A030101(n)) – XOR of n and its binary reversal
 * Range:     u8 n <= 255, u16 n <= 9941, u32 n <= 9941, u64 n <= 9941,
 *            u128 n <= 9941, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t reverse_bits(seq_t x)
{
    seq_t rev = 0;
    while (x)
    {
        rev = (rev << 1) | (x & 1);
        x >>= 1;
    }
    return rev;
}

seq_t a(idx_t n)
{
    seq_t nx = (seq_t)n;
    seq_t rev = reverse_bits(nx);
    return nx ^ rev;
}

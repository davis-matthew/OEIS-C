/*
 * A215256  Longest palindromic factor of (n base 2); in case of tie choose
 *          largest; if it begins with 0 complement it.
 * https://oeis.org/A215256
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 32, u32 n <= 1024, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A215256:
   Longest palindromic factor of the binary representation of n,
   choosing the lexicographically largest in case of ties,
   and complementing the resulting bit string if it starts with 0. */
seq_t a(idx_t n)
{
    // Bit array for the binary representation of n
    int bits[64];
    int len = 0;
    idx_t temp = n;
    if (temp == 0)
        len = 1;
    else {
        while (temp) {
            temp >>= 1;
            ++len;
        }
    }
    // Fill bits[0] with the most significant bit
    for (int i = 0; i < len; ++i)
        bits[i] = (int)((n >> (len - 1 - i)) & 1);

    // Search for longest palindrome
    int bestLen = 0, bestL = 0, bestR = 0;

    for (int center = 0; center < len; ++center) {
        // Odd length palindromes
        int l = center, r = center;
        while (l >= 0 && r < len && bits[l] == bits[r]) {
            int candLen = r - l + 1;
            if (candLen > bestLen) {
                bestLen = candLen;
                bestL = l;
                bestR = r;
            } else if (candLen == bestLen) {
                // Lexicographic comparison (same length)
                for (int k = 0; k < candLen; ++k) {
                    int bl = bits[l + k];
                    int bb = bits[bestL + k];
                    if (bl > bb) {
                        bestL = l;
                        bestR = r;
                        break;
                    } else if (bl < bb) {
                        break;
                    }
                }
            }
            --l;
            ++r;
        }

        // Even length palindromes
        l = center;
        r = center + 1;
        while (l >= 0 && r < len && bits[l] == bits[r]) {
            int candLen = r - l + 1;
            if (candLen > bestLen) {
                bestLen = candLen;
                bestL = l;
                bestR = r;
            } else if (candLen == bestLen) {
                for (int k = 0; k < candLen; ++k) {
                    int bl = bits[l + k];
                    int bb = bits[bestL + k];
                    if (bl > bb) {
                        bestL = l;
                        bestR = r;
                        break;
                    } else if (bl < bb) {
                        break;
                    }
                }
            }
            --l;
            ++r;
        }
    }

    // Build the final value, complementing if the first bit is 0
    seq_t result = 0;
    int startBit = bits[bestL];
    for (int i = bestL; i <= bestR; ++i) {
        int bit = bits[i];
        if (startBit == 0)
            bit = 1 - bit;   // complement
        result = result * 10 + bit;   // digits are 0 or 1
    }
    return result;
}

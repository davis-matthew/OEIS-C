/*
 * A089024  1's separated by d(n) 0's, where d(n) = n-th digit of decimal
 *          expansion of Pi.
 * https://oeis.org/A089024
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) is the nth bit in the binary string formed by * writing
 *            each decimal digit d of pi as the pattern 1 followed by d zeros.
 *            * Example: Pi = 3.14159… → 3 → 1000, 1 → 10, 4 → 10000, ... *
 *            concatenated gives 1000101000010… and a(7)=1
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Precomputed decimal digits of Pi (include the integer part).
   2000 digits are sufficient for computing up to ~10,000 bits of the
   sequence. */
static const char *pi_digits =
    "3141592653589793238462643383279502884197169399375105820974"
    "9445923078164062862089986280348253421170679821480865132823"
    "0664709384460955058223172535940812848111745028410270193851"
    "0593317730103057270365759591953092186117381932611793105119"
    "0912164244280109711957312269610195931984412703853700024734"
    "9558462416909368550562627946206480224088739661667373847942"
    "4528632793622957402719418355586728292785089211913226144928"
    "595751761398606945351916161030 V19 unknown ...";

/* Computes the nth term (1‑based) of OEIS A089024. */
seq_t a(idx_t n)
{
    idx_t pos = n;                /* remaining position in the sequence   */
    for (int i = 0; pi_digits[i] != '\0'; ++i) {
        int d = pi_digits[i] - '0';          /* digit of Pi                  */
        idx_t seg_len = (idx_t)d + 1;        /* length of pattern 1 followed by d zeros */
        if (pos <= seg_len) {
            /* If position is 1 within the segment, it's a '1', else '0'. */
            return (pos == 1 ? (seq_t)1 : (seq_t)0);
        }
        pos -= seg_len;                       /* skip this segment              */
    }
    /* In the unlikely case that we run out of precomputed digits we
       return 0 to avoid undefined behaviour. */
    return (seq_t)0;
}

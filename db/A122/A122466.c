/*
 * A122466  Semiprimes written in base 2.
 * https://oeis.org/A122466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = A007088(A001358(n)) – binary representation of the n‑th
 *            semiprime
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Use a simple sieve to generate the n‑th semiprime.
 * The limit n*10+100 is sufficient for n up to at least 100 000.
 * The binary representation is produced by extracting bits from right
 * to left and assembling the corresponding decimal digits. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_spf.h"   /* provides void build_spf(idx_t limit, idx_t **spf_out); */

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* offset starts at 1 */

    /* Rough upper bound for the nth semiprime (sufficient for n <= 100 000). */
    idx_t limit = n * 10 + 100;

    /* Allocate smallest prime factor (SPF) array. */
    idx_t *spf = NULL;
    build_spf(limit, &spf);
    if (!spf) return 0;                  /* allocation or algorithm failure */

    /* Array of the number of prime factors (with multiplicity) for each i. */
    uint8_t *fcount = (uint8_t*)malloc((limit + 1) * sizeof(uint8_t));
    if (!fcount) { free(spf); return 0; }
    fcount[0] = 0;
    fcount[1] = 0;

    idx_t count = 0;                     /* how many semiprimes seen so far */
    idx_t semiprime = 0;                 /* the n‑th semiprime */

    for (idx_t i = 2; i <= limit; ++i) {
        idx_t s = spf[i];
        idx_t m = i / s;
        uint8_t fc = fcount[m] + 1;      /* number of prime factors of i */
        fcount[i] = fc;
        if (fc == 2) {
            ++count;
            semiprime = i;
            if (count == n) break;       /* found the desired semiprime */
        }
    }

    free(fcount);
    free(spf);

    if (n > count) return 0;              /* not found – should not happen */

    /* Convert the integer  semiprime  to a decimal integer whose
     * digits are the binary digits of semiprime. */
    seq_t result = 0;
    seq_t pow10   = 1;
    idx_t temp    = semiprime;
    while (temp) {
        seq_t digit  = (temp & 1);
        result += digit * pow10;
        pow10   *= (seq_t)10;
        temp >>= 1;
    }

    return result;
}

/*
 * A131618  Numbers k such that the decimal expansion of 3^k contains no 4.
 * https://oeis.org/A131618
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, nonn
 * Formula:   a(n) is the n-th integer k such that 3ⁿ contains no digit 4 in
 *            its decimal expansion
 * Range:     u8 n <= 17, u16 n <= 17, u32 n <= 17, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state for caching the sequence and the decimal power of 3 */
    static seq_t *terms   = NULL;   /* array of found indices k */
    static size_t tsize   = 0;      /* number of terms stored */
    static size_t tcap    = 0;      /* allocated capacity of terms */
    static idx_t  curr_k  = 0;      /* current exponent k we are at */

    static uint8_t *digs  = NULL;   /* decimal digits (little‑endian) of 3^curr_k */
    static size_t   dlen  = 0;      /* number of digits */

    /* initialisation */
    if (terms == NULL) {
        tcap = 32;
        terms = (seq_t *)malloc(tcap * sizeof(seq_t));
        terms[0] = (seq_t)0;     /* a(1) = 0 */
        tsize = 1;

        dlen = 1;
        digs = (uint8_t *)malloc(1 * sizeof(uint8_t));
        digs[0] = 1;             /* 3^0 = 1 */
    }

    if (n == 0)   /* undefined, but keep the function type consistent */
        return (seq_t)0;

    /* generate terms until we have the n‑th one */
    while (tsize < n) {
        bool found = false;
        while (!found) {
            /* advance to next exponent */
            curr_k++;

            /* multiply digs by 3 */
            uint32_t carry = 0;
            for (size_t i = 0; i < dlen; ++i) {
                uint32_t val = (uint32_t)digs[i] * 3 + carry;
                digs[i] = (uint8_t)(val % 10);
                carry   = val / 10;
            }
            while (carry) {
                digs = (uint8_t *)realloc(digs, dlen + 1);
                digs[dlen++] = (uint8_t)(carry % 10);
                carry /= 10;
            }

            /* check for the presence of digit 4 */
            found = true;
            for (size_t i = 0; i < dlen; ++i) {
                if (digs[i] == 4) {
                    found = false;
                    break;
                }
            }
        }

        /* store the found exponent */
        if (tsize == tcap) {
            tcap = tcap ? tcap * 2 : 32;
            terms = (seq_t *)realloc(terms, tcap * sizeof(seq_t));
        }
        terms[tsize++] = (seq_t)curr_k;
    }

    return terms[n - 1];
}

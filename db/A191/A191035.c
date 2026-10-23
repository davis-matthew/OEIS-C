/*
 * A191035  Primes that are squares mod 53.
 * https://oeis.org/A191035
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime p (p ≠ 53) such that p is a quadratic
 *            residue modulo 53. The sequence is sorted in increasing order
 * Range:     u8 n <= 23, u16 n <= 3229, u32 n <= 7480, u64 n <= 7480,
 *            u128 n <= 7480, big any n
 * Helpers:   is_prime, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* List of non‑zero quadratic residues modulo 53.
       residues[i] == 1  ⇔  i is a quadratic residue (mod 53). */
    static const uint8_t residues[53] = {
        0, /*0*/
        1, /*1*/
        0, /*2*/
        0, /*3*/
        1, /*4*/
        0, /*5*/
        1, /*6*/
        1, /*7*/
        0, /*8*/
        1, /*9*/
        1, /*10*/
        1, /*11*/
        0, /*12*/
        1, /*13*/
        0, /*14*/
        1, /*15*/
        1, /*16*/
        1, /*17*/
        0, /*18*/
        0, /*19*/
        0, /*20*/
        0, /*21*/
        0, /*22*/
        0, /*23*/
        1, /*24*/
        1, /*25*/
        0, /*26*/
        0, /*27*/
        1, /*28*/
        1, /*29*/
        0, /*30*/
        0, /*31*/
        0, /*32*/
        0, /*33*/
        0, /*34*/
        0, /*35*/
        1, /*36*/
        1, /*37*/
        1, /*38*/
        0, /*39*/
        1, /*40*/
        0, /*41*/
        1, /*42*/
        1, /*43*/
        1, /*44*/
        0, /*45*/
        1, /*46*/
        1, /*47*/
        0, /*48*/
        1, /*49*/
        0, /*50*/
        0, /*51*/
        1  /*52*/
    };

    /* Static cache of qualifying primes. */
    static seq_t *cache  = NULL;
    static size_t len    = 0;
    static size_t cap    = 0;
    static idx_t candidate = 2;          /* next integer to test */

    /* Grow cache until we have at least n entries. */
    while (len < n) {
        /* Find next prime that is a quadratic residue modulo 53. */
        for ( ; ; ++candidate) {
            if (is_prime(candidate)) {
                if (residues[(uint8_t)(candidate % 53)]) {
                    if (len == cap) {
                        cap = cap ? cap * 2 : 8;
                        cache = seq_realloc(cache, len, cap);
                    }
                    cache[len++] = (seq_t)candidate;
                    /* Advance candidate to avoid finding the same prime again. */
                    ++candidate;
                    break;
                }
            }
        }
    }

    return cache[n - 1];
}

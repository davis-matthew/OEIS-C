/*
 * A058179  Numbers whose English names include all five vowels at least once.
 * https://oeis.org/A058179
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, word
 * Formula:   not stated
 * Range:     u8 n <= 51, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A058179: Numbers whose English names contain all five vowels at least once */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Vowel bit masks */
#define V_A (1U << 0)
#define V_E (1U << 1)
#define V_I (1U << 2)
#define V_O (1U << 3)
#define V_U (1U << 4)
#define FULL_MASK (V_A | V_E | V_I | V_O | V_U)

/* Masks for individual words (pre‑computed) */
static const uint8_t unitMask[10] = {
    0,                                  /* "" */
    (V_O | V_E),                       /* one      */
    V_U,                               /* two      */
    V_E,                               /* three    */
    (V_O | V_U),                       /* four     */
    (V_I | V_E),                       /* five     */
    V_I,                               /* six      */
    V_E,                               /* seven    */
    (V_I | V_E),                       /* eight    */
    (V_I | V_E)                        /* nine     */
};

static const uint8_t teenMask[10] = {
    V_E,                               /* ten      */
    V_E,                               /* eleven   */
    V_E,                               /* twelve   */
    (V_I | V_E),                       /* thirteen */
    (V_O | V_U | V_E),                 /* fourteen */
    (V_I | V_E),                       /* fifteen  */
    (V_I | V_E),                       /* sixteen  */
    V_E,                               /* seventeen */
    (V_I | V_E),                       /* eighteen */
    (V_I | V_E)                        /* nineteen */
};

static const uint8_t tensMask[10] = {
    0,                                  /* "" */
    0,                                  /* "" */
    V_E,                                /* twenty   */
    V_I,                                /* thirty   */
    V_O,                                /* forty    */
    V_I,                                /* fifty    */
    V_I,                                /* sixty    */
    V_E,                                /* seventy  */
    (V_I | V_E),                        /* eighty   */
    (V_I | V_E)                         /* ninety   */
};

static const uint8_t scaleMask[7] = {
    0,                                    /* "" placeholder */
    (V_O | V_U | V_A),                    /* thousand   */
    (V_I | V_O),                          /* million    */
    (V_I | V_O),                          /* billion    */
    (V_I | V_O),                          /* trillion   */
    (V_U | V_A | V_I | V_O),              /* quadrillion*/
    (V_U | V_I | V_O)                     /* quintillion*/
};

static const uint8_t hundredMask = (V_U | V_E);  /* hundred   */
static const uint8_t andMask     = V_A;          /* and       */

/* Compute vowel mask of the English representation of num (1‑based) */
static uint8_t computeMask(uint64_t num) {
    uint8_t mask = 0;
    /* Split the number into groups of three digits */
    uint64_t groups[7] = {0};
    int g = 0;
    uint64_t n = num;
    while (n > 0 && g < 7) {
        groups[g++] = n % 1000;
        n /= 1000;
    }

    bool higherNonZero = false;  /* any higher group non‑zero? */

    for (int i = g - 1; i >= 0; --i) {
        uint32_t val = (uint32_t)groups[i];
        if (val == 0)
            continue;

        /* Insert 'and' before the final group if needed (British usage) */
        if (i == 0 && higherNonZero && val < 100)
            mask |= andMask;

        if (val >= 100) {
            uint32_t hundreds = val / 100;
            mask |= unitMask[hundreds];
            mask |= hundredMask;

            uint32_t rem = val % 100;
            if (rem > 0) {
                mask |= andMask; /* British usage */
                if (rem >= 20) {
                    uint32_t t = rem / 10;
                    mask |= tensMask[t];
                    uint32_t u = rem % 10;
                    if (u > 0)
                        mask |= unitMask[u];
                } else if (rem >= 10) {
                    mask |= teenMask[rem - 10];
                } else { /* 1‑9 */
                    mask |= unitMask[rem];
                }
            }
        } else { /* <100 */
            if (val >= 20) {
                uint32_t t = val / 10;
                mask |= tensMask[t];
                uint32_t u = val % 10;
                if (u > 0)
                    mask |= unitMask[u];
            } else if (val >= 10) {
                mask |= teenMask[val - 10];
            } else { /* 1‑9 */
                mask |= unitMask[val];
            }
        }
        if (i > 0)            /* add scale word except units */
            mask |= scaleMask[i];

        higherNonZero = true; /* mark that a higher group has been processed */
    }

    return mask;
}

/* Memoised array of found numbers (stored as uint64_t) */
static uint64_t *cacheNum = NULL;
static idx_t cacheSize = 0;
static idx_t cacheCap = 0;
static uint64_t nextNum = 1;  /* next number to test */

/* Main sequence function */
seq_t a(idx_t n) {
    /* Generate terms until we have at least n */
    while (cacheSize < n) {
        uint8_t mask = computeMask(nextNum);
        if (mask == FULL_MASK) {
            if (cacheSize == cacheCap) {
                cacheCap = (cacheCap == 0) ? 64 : cacheCap * 2;
                uint64_t *tmp = (uint64_t *)realloc(cacheNum, cacheCap * sizeof(uint64_t));
                cacheNum = tmp;
            }
            cacheNum[cacheSize++] = nextNum;
        }
        ++nextNum;
    }
    return (seq_t)cacheNum[n - 1];
}

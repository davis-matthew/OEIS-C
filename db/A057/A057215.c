/*
 * A057215  [1->01, 2->10, 3->01]-transform of 3-symbol Thue-Morse A026600.
 * https://oeis.org/A057215
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A057215: 1→01, 2→10, 3→01 transform of the 3‑symbol Thue–Morse A026600
 *  The 3‑symbol Thue–Morse sequence t(k) (k = ⌊n/2⌋) follows the morphism
 *      1 → 1 2 3
 *      2 → 2 3 1
 *      3 → 3 1 2
 *  Each symbol expands to two bits:
 *      1 → 0 1
 *      2 → 1 0
 *      3 → 0 1
 *  Thus for n:
 *      k = ⌊n/2⌋ , r = n mod 2
 *      a(n) = 1   if (t(k)==2 && r==0)  or  (t(k)!=2 && r==1)
 *             0   otherwise
 *
 *  This implementation precomputes t(k) on demand up to the required index.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Static storage for the 3‑symbol Thue–Morse sequence */
static uint8_t *t_seq    = NULL;   /* symbols: 1,2,3 */
static size_t   t_len    = 0;      /* current length of t_seq           */

/* Expand t_seq until it contains at least (m+1) symbols. */
static void ensure_t_seq(idx_t m)
{
    /* Initialise if necessary */
    if (t_seq == NULL)
    {
        t_seq = (uint8_t *)malloc(1);
        if (t_seq == NULL) abort();          /* defensive: should never happen */
        t_seq[0] = 1;                         /* first symbol of t_0    */
        t_len = 1;
    }

    /* Grow until we have enough symbols */
    while ((size_t)m >= t_len)
    {
        size_t new_len = 3 * t_len;
        uint8_t *new_seq = (uint8_t *)malloc(new_len);
        if (new_seq == NULL) abort();          /* defensive: should never happen */

        /* Expand each existing symbol according to the morphism */
        for (size_t i = 0; i < t_len; ++i)
        {
            uint8_t s = t_seq[i];
            size_t j = 3 * i;
            new_seq[j] = s;
            switch (s)
            {
                case 1: new_seq[j+1] = 2; new_seq[j+2] = 3; break;
                case 2: new_seq[j+1] = 3; new_seq[j+2] = 1; break;
                case 3: new_seq[j+1] = 1; new_seq[j+2] = 2; break;
            }
        }

        free(t_seq);
        t_seq = new_seq;
        t_len = new_len;
    }
}

/* External linkage function required by the build */
seq_t a(idx_t n)
{
    /* Find the corresponding symbol index and even/odd position */
    idx_t k       = n >> 1;        /* floor(n/2) */
    bool  even    = ( (n & 1) == 0 );   /* true if n is even */

    /* Ensure the symbol for index k is available */
    ensure_t_seq(k);

    /* Retrieve the symbol (1,2,3) */
    uint8_t symbol = t_seq[(size_t)k];

    /* 'basebit' is 1 when the symbol is 2, otherwise 0 */
    bool basebit = (symbol == 2);

    /* a(n)=1 iff (basebit == even), else 0 */
    return (seq_t)( basebit == even );
}

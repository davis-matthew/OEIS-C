/*
 * A082516  Differences between consecutive Niven (or Harshad) numbers.
 * https://oeis.org/A082516
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = A005349(n+1) – A005349(n) (differences of consecutive
 *            Niven numbers)
 * Range:     u8 n <= 75, u16 n <= 9061, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_niven
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_niven.h"

/* Dynamic array of Niven numbers, 1‑based: N[0] = 1, N[1] = 2, ...            */
static seq_t *Niven = NULL;
static size_t NivenCap = 0;
static idx_t   NivenCount = 0;   /* number of Niven values stored   */
static seq_t   LastChecked = 0;  /* the largest number examined so far */

/* Ensure the array can hold at least 'need' elements  */
static void grow_niven(size_t need)
{
    if (need <= NivenCap) return;
    size_t newcap = NivenCap ? 2 * NivenCap : 8;
    while (newcap < need) newcap *= 2;
    Niven = (seq_t *)realloc(Niven, newcap * sizeof(seq_t));
    NivenCap = newcap;
}

/* Expand the list so that it contains at least 'up_to' Niven numbers (0‑based)  */
static void extend_niven(idx_t up_to)
{
    while (NivenCount <= up_to) {
        LastChecked++;
        if (is_niven(LastChecked)) {
            grow_niven(NivenCount + 1);
            Niven[NivenCount++] = LastChecked;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* undefined, but safe */
    /* need N_{n} and N_{n+1} => indices n-1 and n in 0‑based array  */
    extend_niven(n);          /* now we have N[0..n]   */
    return Niven[n] - Niven[n-1];
}

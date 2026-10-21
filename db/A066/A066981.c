/*
 * A066981  Number of residues of Lucas numbers modulo n.
 * https://oeis.org/A066981
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct residues of the Lucas sequence modulo
 *            n Lucas numbers: L(0)=2, L(1)=1, L(k)=L(k-1)+L(k-2) (k≥2)
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Compute the number of distinct residues of Lucas numbers modulo n
   for n ≥ 1.  The algorithm generates the Lucas sequence modulo n,
   records each distinct residue in a boolean array, and stops when
   the initial pair (L0,L1) reappears, which guarantees that all
   residues that will ever occur have already been seen.  This runs
   in O(n) time and uses O(n) memory for n up to about 10 000.       */
seq_t a(idx_t n)
{
    uint64_t mod = (uint64_t)n;
    if (mod == 0) return 0;           /* safety, though n≥1 in the OEIS */

    uint64_t curr   = 2 % mod;        /* L0 */
    uint64_t next   = 1 % mod;        /* L1 */
    uint64_t init_c = curr;           /* remember initial pair */
    uint64_t init_n = next;

    /* allocate seen array of size mod bytes */
    bool *seen = (bool *)malloc(mod * sizeof(bool));
    if (seen == NULL) return 0;       /* allocation failure (unlikely) */
    memset(seen, 0, mod * sizeof(bool));

    size_t count = 0;
    do
    {
        if (!seen[curr])
        {
            seen[curr] = true;
            ++count;
        }
        uint64_t new_next = (curr + next) % mod;
        curr = next;
        next = new_next;
    } while (!(curr == init_c && next == init_n));

    free(seen);
    return (seq_t)count;
}

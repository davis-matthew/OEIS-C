/*
 * A223487  Number of missing residues in Lucas sequence mod n.
 * https://oeis.org/A223487
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of residues modulo n that never appear in *
 *            the Lucas sequence (L0 = 2, L1 = 1, Lk+2 = Lk+1 + Lk). The
 *            function * simulates the Lucas sequence modulo n, marks every
 *            residue seen, and * returns the count of residues not marked. It
 *            works for any seq_t and * idx_t provided by the...
 * Range:     u8 n <= 274, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Header files:  */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/*  Static buffer reused between calls  */
static uint8_t *seen = NULL;
static size_t seen_cap = 0;

/*  Number of missing residues in Lucas sequence modulo n  */
seq_t a(idx_t n)
{
    if (n == 0)          /* offset 1: a(1) is first term; n=0 ignored */
        return (seq_t)0;

    size_t sz = (size_t)n;
    if (seen_cap < sz) {                 /* grow buffer if necessary */
        uint8_t *tmp = (uint8_t *)realloc(seen, sz);
        if (!tmp) return (seq_t)0;       /* allocation failed */
        seen = tmp;
        seen_cap = sz;
    }
    memset(seen, 0, sz);                 /* clear previous data */

    uint64_t mod = (uint64_t)n;
    uint64_t L0 = 2 % mod;
    uint64_t L1 = 1 % mod;

    seen[(size_t)L0] = 1;                /* mark initial residues */
    seen[(size_t)L1] = 1;

    uint64_t prev = L0;
    uint64_t curr = L1;

    /*  Simulate until we return to the starting pair (L0, L1)  */
    do {
        uint64_t next = (uint64_t)(((__uint128_t)prev + curr) % mod);
        if (!seen[(size_t)next]) seen[(size_t)next] = 1;
        prev = curr;
        curr = next;
    } while (!(prev == L0 && curr == L1));

    /*  Count residues that never appeared  */
    seq_t missing = 0;
    for (idx_t i = 0; i < n; ++i)
        if (!seen[(size_t)i]) ++missing;

    return missing;
}

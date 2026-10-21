/*
 * A060030  a(1) = 1, a(2) = 2; thereafter a "hole" is defined to be any
 *          positive number not in the sequence a(1)..a(n-1) and less than the
 *          largest term; if there exists at least one hole, then a(n) is the
 *          largest hole, otherwise a(n) = a(n-2) + a(n-1).
 * https://oeis.org/A060030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) is defined recursively: * a(1)=1, a(2)=2. * For n>2: let L
 *            be the largest positive integer not yet appearing in *
 *            a(1)..a(n-1) that is < max{a(1)..a(n-1)}. * If such an L exists,
 *            a(n)=L; otherwise a(n)=a(n-2)+a(n-1)
 * Range:     u8 n <= 253, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/seq_alloc.h"     /* seq_alloc(size_t) */
#include "../utils/seq_realloc.h"   /* seq_realloc(seq_t*,size_t,size_t) */

static seq_t *seq          = NULL;   /* 1‑based array of terms   */
static size_t seq_cap      = 0;      /* allocated size of seq    */
static uint8_t *present    = NULL;   /* bitmap: present[i]==1 iff i seen   */
static size_t present_cap  = 0;      /* allocated size of present          */
static idx_t maxComputed   = 0;      /* highest index computed so far      */
static seq_t maxUsed       = (seq_t)0;/* largest value seen                 */

/* enlarge seq array to at least new_cap elements (1‑based) */
static void ensure_seq_capacity(size_t new_cap)
{
    if (seq_cap >= new_cap) return;
    size_t old_cap = seq_cap ? seq_cap : 1;
    seq = seq_realloc(seq, old_cap, new_cap);
    seq_cap = new_cap;
}

/* enlarge present array to at least val+1 elements (indexed by size_t) */
static void ensure_present_capacity(seq_t val)
{
    size_t need = (size_t)val + 1;          /* index val is possible */
    if (present_cap >= need) return;
    size_t old_cap = present_cap ? present_cap : 1;
    uint8_t *tmp = (uint8_t *)realloc(present, need * sizeof(uint8_t));
    if (!tmp) abort();
    /* zero new area */
    if (need > old_cap)
        memset(tmp + old_cap, 0, (need - old_cap) * sizeof(uint8_t));
    present = tmp;
    present_cap = need;
}

seq_t a(idx_t n)
{
    /* lazy global initialisation on first call */
    if (!seq) {
        /* minimal capacities */
        seq_cap = 4;
        seq = seq_alloc(seq_cap);
        memset(seq, 0, seq_cap * sizeof(seq_t));

        seq[1] = (seq_t)1;
        seq[2] = (seq_t)2;
        maxComputed = 2;
        maxUsed = (seq_t)2;

        present_cap = 4;
        present = (uint8_t *)malloc(present_cap * sizeof(uint8_t));
        if (!present) abort();
        memset(present, 0, present_cap * sizeof(uint8_t));
        present[1] = 1;
        present[2] = 1;
    }

    if (n == 0) return (seq_t)0; /* not defined but harmless */

    if (n <= maxComputed) return seq[n];

    /* compute missing terms up to n */
    for (idx_t m = maxComputed + 1; m <= n; ++m) {
        seq_t val;

        /* search for the largest hole < maxUsed */
        if (maxUsed <= (seq_t)1) {
            /* no possible hole */
            val = seq[m-2] + seq[m-1];
        } else {
            /* scan downward from maxUsed-1 */
            size_t pos = (size_t)maxUsed - 1;
            while (pos > 0 && present[pos]) --pos;
            if (pos > 0) {
                /* hole found */
                val = (seq_t)pos;
            } else {
                /* no hole */
                val = seq[m-2] + seq[m-1];
            }
        }

        /* mark new value as present, expanding bitmap if necessary */
        ensure_present_capacity(val);
        present[(size_t)val] = 1;
        if (val > maxUsed) maxUsed = val;

        /* store term */
        ensure_seq_capacity((size_t)m + 1); /* need index m */
        seq[m] = val;
        maxComputed = m;
    }

    return seq[n];
}

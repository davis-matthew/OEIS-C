/*
 * A039855  Number of partitions satisfying cn(2,5) <= 1 and cn(3,5) <= 1.
 * https://oeis.org/A039855
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer partitions of n in which at most one
 *            part is congruent to 2 (mod 5) and at most one part is congruent
 *            to 3 (mod 5). This is implemented by multiplying the generating
 *            function: G(x) = (1 + Σ_{m≡2(5)} x^m) · (1 + Σ_{m≡3(5)} x^m) ·
 *            ∏_{m≠2,3 (mod 5)} 1/(1−x^m)
 * Range:     u8 n <= 20, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses two working arrays of size n+1.  The arrays
   are resized only when a larger n is requested.  The algorithm
   runs in O(n²) time with a small constant factor. */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *dp_unrestricted = NULL;   /* partitions using only parts 0,1,4 (mod 5) */
static seq_t *dp_after2      = NULL;   /* after adding part ≡2 (mod 5) */
static idx_t  stored_size    = 0;       /* current capacity of the arrays (n+1) */

/* Ensure the working arrays can hold indices up to `need`.  The
   arrays are expanded when `need` exceeds the current size. */
static void ensure_size(idx_t need)
{
    if (need <= stored_size) return;

    seq_t *new1 = (seq_t *)realloc(dp_unrestricted, (need + 1) * sizeof(seq_t));
    if (!new1) return;          /* allocation failed – caller will see 0 */
    seq_t *new2 = (seq_t *)realloc(dp_after2,      (need + 1) * sizeof(seq_t));
    if (!new2) {
        dp_unrestricted = new1; /* rollback first array to keep consistency */
        return;
    }
    dp_unrestricted = new1;
    dp_after2      = new2;
    stored_size    = need;
}

/* Main routine computing a(n).  It returns 0 for n==0 (out of range). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Allocate/resize working arrays. */
    ensure_size(n);
    if (!dp_unrestricted || !dp_after2) return 0;   /* OOM */

    /* 1. Compute partitions using unrestricted parts (residues 0,1,4). */
    dp_unrestricted[0] = 1;
    for (idx_t i = 1; i <= n; ++i) dp_unrestricted[i] = 0;

    for (idx_t s = 1; s <= n; ++s) {
        if (s % 5 == 2 || s % 5 == 3) continue;        /* restricted parts */
        for (idx_t i = s; i <= n; ++i)
            dp_unrestricted[i] += dp_unrestricted[i - s];
    }

    /* 2. Add exactly one part from residue 2 group. */
    for (idx_t i = 0; i <= n; ++i) dp_after2[i] = dp_unrestricted[i];

    for (idx_t m = 2; m <= n; m += 5) {               /* all m ≡ 2 (mod 5) */
        for (idx_t i = m; i <= n; ++i)
            dp_after2[i] += dp_unrestricted[i - m];
    }

    /* 3. Add exactly one part from residue 3 group using dp_after2 as the
       source so that no two parts from this group are chosen. */
    for (idx_t i = 0; i <= n; ++i) dp_unrestricted[i] = dp_after2[i];

    for (idx_t m = 3; m <= n; m += 5) {               /* all m ≡ 3 (mod 5) */
        for (idx_t i = m; i <= n; ++i)
            dp_unrestricted[i] += dp_after2[i - m];
    }

    return dp_unrestricted[n];
}

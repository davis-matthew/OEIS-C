/*
 * A090979  From the sequence of natural numbers, go on deleting n smallest
 *          multiples of n (>n) not deleted earlier; a(n) = the largest number
 *          deleted.
 * https://oeis.org/A090979
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * A090979: a(n) = the largest number deleted when removing the n smallest
 * multiples of n (> n) that have not been deleted earlier.
 *
 * The implementation keeps a global deletion flag array and, for each
 * query n, scans multiples of n until n new numbers are marked deleted.
 * The last number marked is returned as a(n).  This simulates the process
 * directly; there is no known closed‑form formula.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>

/* Global deletion array: del[i] == 1 iff number i has been deleted. */
static unsigned char *del = NULL;   /* 0‑based index: deleted flag */
static size_t del_len = 0;          /* current capacity */

/* Ensure the deletion array can store indices up to `need` (inclusive). */
static void ensure_capacity(size_t need)
{
    if (need < del_len)
        return;
    size_t new_len = del_len ? del_len * 2 : 1024;
    if (new_len <= need)
        new_len = need + 1;
    unsigned char *new_del = (unsigned char *)realloc(del, new_len);
    if (!new_del) abort();          /* out of memory */
    /* New slots are uninitialized; set them to 0 (not deleted). */
    memset(new_del + del_len, 0, new_len - del_len);
    del = new_del;
    del_len = new_len;
}

/* Compute a(n) for n ≥ 1.  a(0) is undefined; return 0 in that case. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    idx_t deletions_needed = n;
    idx_t count = 0;
    seq_t last_deleted = 0;

    /* Search multiples of n, starting from 2*n.  We use a 128‑bit product
       to avoid overflow for n up to ~10⁴; the values stay far below
       the 64‑bit limit. */
    for (idx_t m = 2; count < deletions_needed; ++m)
    {
        __int128 prod128 = (__int128)m * n;
        if (prod128 > (idx_t)-1)   /* safety check, should not occur for given limits */
            break;
        idx_t val = (idx_t)prod128;
        ensure_capacity((size_t)val); /* ensure del can be indexed at val */
        if (!del[val])
        {
            del[val] = 1;
            ++count;
            last_deleted = (seq_t)val;
        }
    }
    return last_deleted;
}

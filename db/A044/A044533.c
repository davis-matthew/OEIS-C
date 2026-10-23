/*
 * A044533  Numbers n such that string 2,0 occurs in the base 7 representation
 *          of n but not of n+1.
 * https://oeis.org/A044533
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A044533: numbers n whose base‑7 representation contains
   the substring "20" but n+1 does not.  The implementation searches for such numbers
   sequentially, memoising found terms so that subsequent calls are fast. */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

/* Test whether a number contains the pattern 2,0 in base 7. */
static bool has20(seq_t x)
{
    if (x == 0) return false;

    /* Collect base‑7 digits (least significant first).  */
    unsigned char digits[256];
    size_t len = 0;
    while (x > 0) {
        digits[len++] = (unsigned char)(x % 7);
        x = x / 7;
    }

    /* Scan for a 2 followed by a 0 in more significant position,
       i.e. digits[i] == 2 && digits[i-1] == 0. */
    for (size_t i = 1; i < len; ++i) {
        if (digits[i] == 2 && digits[i-1] == 0) {
            return true;
        }
    }
    return false;
}

#ifdef __cplusplus
/* C++ implementation: use a static vector for storage. */
static std::vector<seq_t> seq_storage;
static seq_t next_search = 1;

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* behaviour below offset is irrelevant */

    while (seq_storage.size() < (size_t)n) {
        seq_t z = next_search;
        for (;;) {
            if (has20(z) && !has20(z + 1)) {
                seq_storage.push_back(z);
                next_search = z + 1;
                break;
            }
            ++z;
        }
    }
    return seq_storage[(size_t)(n - 1)];
}
#else
/* C implementation: use a malloc'd array for storage. */
static seq_t *seq_storage = NULL;
static size_t seq_len   = 0;
static size_t seq_cap   = 0;
static seq_t next_search = 1;

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* behaviour below offset is irrelevant */

    while (seq_len < (size_t)n) {
        if (seq_len >= seq_cap) {
            size_t new_cap = seq_cap ? (seq_cap << 1) : 128;
            seq_t *tmp = (seq_t *)realloc(seq_storage, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;     /* allocation failure – return zero */
            seq_storage = tmp;
            seq_cap = new_cap;
        }
        seq_t z = next_search;
        for (;;) {
            if (has20(z) && !has20(z + 1)) {
                seq_storage[seq_len++] = z;
                next_search = z + 1;
                break;
            }
            ++z;
        }
    }
    return seq_storage[(size_t)(n - 1)];
}
#endif

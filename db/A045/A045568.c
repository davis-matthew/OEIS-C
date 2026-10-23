/*
 * A045568  Numbers whose factorial has '88' as its final two digits before
 *          the trailing zeros.
 * https://oeis.org/A045568
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 3317, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A045568. a(n) is the n‑th positive integer k such that
   when k! is divided by the largest power of 10 that divides it
   (i.e. the trailing zeros are removed), the resulting integer ends
   with the two digits 88.  The function searches for such k, keeping
   a running product modulo 100 and the net excess of 2’s over 5’s
   counted in the factorial.  All arithmetic that remains in the
   implementation uses only integers up to 64 bits – the big‑integer
   type seq_t is only used for storing the final return value.  */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* --- static storage for the pre‑computed sequence ------------------- */
static seq_t *seq       = NULL;          /* array of found k values      */
static size_t seq_len  = 0;              /* number of elements stored   */
static size_t seq_cap  = 0;              /* current capacity            */

/* --- running state of the factorial calculation --------------------- */
static idx_t last_x = 0;                 /* greatest k processed so far */
static idx_t p2     = 0;                 /* total exponent of 2 seen    */
static idx_t p5     = 0;                 /* total exponent of 5 seen    */
static uint64_t cur = 1;                 /* product of reduced terms mod 100 */

/* --- table of powers of 2 modulo 100 -------------------------------- */
static unsigned char *pow2_mod = NULL;   /* pow2_mod[e] = 2^e (mod 100) */
static size_t pow2_cap = 0;              /* current size of pow2_mod     */

/* enlarge pow2_mod to contain at least indices 0 .. need-1 */
static void ensure_pow2(size_t need)
{
    if (need < pow2_cap) return;
    size_t new_cap = pow2_cap ? pow2_cap * 2 : 4;
    while (new_cap <= need) new_cap <<= 1;
    pow2_mod = (unsigned char *)realloc(pow2_mod, new_cap * sizeof(unsigned char));
    for (size_t i = pow2_cap; i < new_cap; ++i) {
        if (i == 0) pow2_mod[i] = 1;
        else        pow2_mod[i] = (pow2_mod[i-1] * 2) % 100;
    }
    pow2_cap = new_cap;
}

/* enlarge seq array as needed */
static void ensure_seq_capacity(void)
{
    if (seq_len == seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 64;
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined – keep behaviour safe */

    /* find enough terms up to the requested index */
    while (seq_len < (size_t)n) {
        ++last_x;
        idx_t t = last_x;

        /* strip all factors of 2 and 5 from t */
        while ((t & 1u) == 0) {  /* faster even test */
            t >>= 1;
            ++p2;
        }
        while (t % 5 == 0) {
            t /= 5;
            ++p5;
        }

        /* multiply the remaining part modulo 100 */
        cur = (cur * (t % 100)) % 100;

        /* apply the excess of 2's: multiply by 2^(p2-p5) modulo 100 */
        size_t diff = (size_t)(p2 - p5);
        ensure_pow2(diff);
        uint64_t value = (cur * pow2_mod[diff]) % 100;

        if (value == 88) {
            ensure_seq_capacity();
            seq[seq_len++] = (seq_t)last_x;
        }
    }
    return seq[n-1];
}

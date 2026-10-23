/*
 * A163396  Primes in carryless arithmetic mod 10 in which all digits except
 *          the rightmost are even.
 * https://oeis.org/A163396
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   irreducible_mod_prime, is_emod_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A163396: e‑type primes (carryless primes with all digits
   except the last even).  The function generates all such numbers in
   ascending order and tests carryless primality by verifying that the
   corresponding polynomial over Z/10Z is irreducible modulo 2 and
   modulo 5 (a prime in the product ring is prime in each component). */

#include <stdbool.h>
#include "../utils/irreducible_mod_prime.h"
#include "../utils/is_emod_prime.h"

/* Recursive generation of e‑type numbers.  Results are produced in ascending
   numeric order.  When the nth term is found the global flag stops further
   recursion. */
static idx_t target_n;
static idx_t count_seen;
static seq_t result_val;
static bool done_flag;

static void dfs_gen(int pos, int len, seq_t val)
{
    if (done_flag) return;
    if (pos == len) {
        if (is_emod_prime(val)) {
            ++count_seen;
            if (count_seen == target_n) {
                result_val = val;
                done_flag = true;
            }
        }
        return;
    }

    if (pos == 0) {                          /* leading (most significant) digit */
        static const int first[4] = {2,4,6,8};
        for (int i = 0; i < 4; ++i) {
            dfs_gen(pos+1, len, val * (seq_t)10 + (seq_t)first[i]);
            if (done_flag) return;
        }
    } else if (pos == len-1) {                /* least significant digit */
        static const int last[5] = {1,3,5,7,9};
        for (int i = 0; i < 5; ++i) {
            dfs_gen(pos+1, len, val * (seq_t)10 + (seq_t)last[i]);
            if (done_flag) return;
        }
    } else {                                  /* middle digits */
        static const int mid[5] = {0,2,4,6,8};
        for (int i = 0; i < 5; ++i) {
            dfs_gen(pos+1, len, val * (seq_t)10 + (seq_t)mid[i]);
            if (done_flag) return;
        }
    }
}

seq_t a(idx_t n)
{
    target_n = n;
    count_seen = 0;
    done_flag = false;

    for (int len = 2; ; ++len)   /* numbers start at length 2 */
    {
        dfs_gen(0, len, (seq_t)0);
        if (done_flag) break;
    }
    return result_val;
}

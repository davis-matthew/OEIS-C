/*
 * A103853  Numbers k such that the string 1000k is the decimal expansion of a
 *          prime number.
 * https://oeis.org/A103853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 69 terms through n = 981 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Find the nth integer k such that 1000*k is prime.
    Returns the nth such k (a(1)=7, a(2)=9, …).  */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

/*  a(n) = the nth integer k with 1000*k prime, n ≥ 1.  */
seq_t a(idx_t n)
{
    /*  Static state retains all found k's between calls.  */
    static idx_t   count = 0;        /* numbers already found   */
    static seq_t * table = NULL;    /* dynamic array of k's    */
    static idx_t   cap   = 0;        /* current capacity        */
    static idx_t   cur_k = 1;        /* next k to test          */
    static uint64_t pow10 = 10ULL;   /* 10^(digits of cur_k)    */

    if (n == 0)      /* undefined for n=0; behaviour irrelevant */
        return 0;

    while (count < n) {
        /*  Update 10^digits when cur_k reaches a new power of ten.  */
        if (cur_k == pow10)
            pow10 *= 10ULL;

        /*  Candidate number: 1000 concatenated with cur_k.  */
        uint64_t candidate = 1000ULL * pow10 + cur_k;

        if (is_prime_u64(candidate)) {
            /*  Ensure capacity for the new entry.  */
            if (count == cap) {
                idx_t newcap = (cap == 0) ? 4 : cap * 2;
                seq_t *tmp = mem_resize(seq_t, table, newcap);
                table = tmp;
                cap   = newcap;
            }
            table[count++] = (seq_t)cur_k;   /* truncate if seq_t too small; defined behaviour */
        }
        ++cur_k;
    }
    return table[n - 1];
}

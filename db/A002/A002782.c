/*
 * A002782  Concatenate the natural numbers, then partition into minimal
 *          strings so that each term divides the next.
 * https://oeis.org/A002782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, nice
 * Formula:   Partition concatenated natural numbers greedily into strings
 *            where each divides the next
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   string_mod, string_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../utils/string_mod.h"
#include "../utils/string_to_seq.h"

seq_t a(idx_t n) {
    if (n == 1) {
        return 1;
    }
    
    char *concat = (char *)malloc(100000);
    size_t concat_len = 0;
    size_t concat_cap = 100000;
    
    unsigned long long num = 1;
    while (concat_len < concat_cap - 100) {
        char buf[50];
        int buflen = sprintf(buf, "%llu", num);
        if (concat_len + buflen >= concat_cap) {
            concat_cap *= 2;
            char *newconcat = (char *)realloc(concat, concat_cap);
            if (!newconcat) {
                free(concat);
                return 0;
            }
            concat = newconcat;
        }
        memcpy(concat + concat_len, buf, buflen);
        concat_len += buflen;
        num++;
    }
    
    char term_str[100000];
    size_t term_len = 1;
    term_str[0] = '1';
    size_t pos = 1;
    
    for (idx_t i = 2; i <= n; i++) {
        seq_t term_val = string_to_seq(term_str, term_len);
        
        size_t next_len = 1;
        int found = 0;
        
        while (pos + next_len <= concat_len) {
            if (string_mod(concat + pos, next_len, term_val) == 0) {
                memcpy(term_str, concat + pos, next_len);
                term_len = next_len;
                pos += next_len;
                found = 1;
                break;
            }
            next_len++;
        }
        
        if (!found && pos < concat_len) {
            while (pos + next_len > concat_len && num < 10000000) {
                char buf[50];
                int buflen = sprintf(buf, "%llu", num);
                if (concat_len + buflen >= concat_cap) {
                    concat_cap *= 2;
                    char *newconcat = (char *)realloc(concat, concat_cap);
                    if (!newconcat) {
                        free(concat);
                        return 0;
                    }
                    concat = newconcat;
                }
                memcpy(concat + concat_len, buf, buflen);
                concat_len += buflen;
                num++;
            }
            
            next_len = 1;
            while (pos + next_len <= concat_len) {
                if (string_mod(concat + pos, next_len, term_val) == 0) {
                    memcpy(term_str, concat + pos, next_len);
                    term_len = next_len;
                    pos += next_len;
                    found = 1;
                    break;
                }
                next_len++;
            }
        }
    }
    
    seq_t result = string_to_seq(term_str, term_len);
    free(concat);
    return result;
}

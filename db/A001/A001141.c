/*
 * A001141  Describe the previous term! (method A - initial term is 5).
 * https://oeis.org/A001141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, nice
 * Formula:   Look-and-say sequence (describe previous term by count then
 *            digit); starts with 5
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)5;
    
    char *current = (char *)malloc(200000);
    char *next = (char *)malloc(200000);
    
    strcpy(current, "5");
    
    for (idx_t i = 1; i < n; i++) {
        int pos = 0;
        int len = (int)strlen(current);
        
        int j = 0;
        while (j < len) {
            char digit = current[j];
            int count = 1;
            while (j + count < len && current[j + count] == digit) {
                count++;
            }
            
            pos += sprintf(next + pos, "%d%c", count, digit);
            j += count;
        }
        next[pos] = '\0';
        
        char *tmp = current;
        current = next;
        next = tmp;
    }
    
    seq_t result = 0;
    for (int i = 0; current[i]; i++) {
        result = result * (seq_t)10 + (seq_t)(current[i] - '0');
    }
    
    free(current);
    free(next);
    return result;
}
